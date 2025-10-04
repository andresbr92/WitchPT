# AsyncMessageSystem Single-Thread Migration Guide

## Overview

Esta guía proporciona un enfoque completo paso a paso para reconstruir el plugin AsyncMessageSystem de Unreal Engine desde cero como una versión de un solo hilo (single-threaded). El objetivo es preservar toda la funcionalidad central de mensajería mientras eliminamos la complejidad del multi-threading, haciendo el plugin más fácil de entender, depurar y mantener.

### ¿Por qué migrar a single-thread?

El sistema original utiliza múltiples hilos para procesar mensajes, lo que introduce complejidad significativa:

- **Sincronización compleja**: Requiere locks, mutexes y variables atómicas
- **Condiciones de carrera**: Bugs difíciles de reproducir y depurar
- **Overhead de rendimiento**: Los locks y operaciones atómicas tienen costo
- **Complejidad de código**: Más difícil de entender y mantener

Al migrar a single-thread, eliminamos toda esta complejidad manteniendo la funcionalidad.

## Conceptos Clave de la Migración

### Diferencias Fundamentales

**Sistema Multi-Thread (Original)**:

- Usa `TMpscQueue` (Multi-Producer Single-Consumer Queue)
- Variables atómicas (`std::atomic<uint32>`) para IDs thread-safe
- `FCriticalSection` y `FScopeLock` para proteger datos compartidos
- `UE::Tasks` y `ENamedThreads` para procesamiento asíncrono
- `ESPMode::ThreadSafe` para shared pointers

**Sistema Single-Thread (Nuevo)**:

- Usa `TArray` simple para colas de mensajes
- Enteros regulares para IDs (no necesitan ser atómicos)
- Sin locks ni critical sections
- Procesamiento basado en tick del game thread únicamente
- `ESPMode::NotThreadSafe` para shared pointers

### Flujo de Procesamiento

En el sistema single-thread, todo el procesamiento ocurre durante el tick del game thread:

1. Se encolan mensajes en arrays simples
2. Durante el tick, se procesan todos los mensajes pendientes
3. Se ejecutan los callbacks de los listeners
4. Se limpian los mensajes procesados

## Prerequisites

- Unreal Engine 5.0+ instalado
- Comprensión básica del desarrollo de plugins en Unreal Engine
- Familiaridad con C++ y el sistema de reflexión de Unreal Engine
- Entendimiento del sistema de tick y world subsystems de Unreal Engine

## Fase 1: Configuración de la Base

### Paso 1: Crear la Estructura del Plugin

**¿Por qué empezamos aquí?**
La estructura del plugin es fundamental y debe ser simplificada desde el inicio. Eliminamos dependencias relacionadas con threading desde el archivo de build para evitar tentaciones de usar APIs multi-thread.

**Diferencias clave con el sistema original:**

- Removemos módulos como "Tasks" del build file
- Simplificamos las dependencias a solo lo esencial
- Nos enfocamos en módulos core de Unreal Engine

Crear la estructura básica del directorio del plugin:

```
AsyncMessageSystem/
├── AsyncMessageSystem.uplugin
├── Source/
│   └── AsyncMessageSystem/
│       ├── AsyncMessageSystem.build.cs
│       ├── Public/
│       └── Private/
```

**1.1 Create AsyncMessageSystem.uplugin**

````json
{
    "FileVersion": 3,
    "Version": 1,
    "VersionName": "1.0",
    "FriendlyName": "Async Message System",
    "Description": "Single-threaded message broadcasting system for Unreal Engine",
    "Category": "Messaging",
    "CreatedBy": "Your Name",
    "CreatedByURL": "",
    "DocsURL": "",
    "MarketplaceURL": "",
    "SupportURL": "",
    "CanContainContent": false,
    "IsBetaVersion": false,
    "IsExperimentalVersion": false,
    "Installed": false,
    "Modules": [
        {
            "Name": "AsyncMessageSystem",
            "Type": "Runtime",
            "LoadingPhase": "Default"
        }
    ]
}
```**1.2 Crear AsyncMessageSystem.build.cs**

**¿Por qué es importante este archivo?**
El archivo .build.cs define las dependencias del módulo. En el sistema original, incluía módulos como "Tasks" para soporte multi-threading. Aquí los eliminamos deliberadamente.

**Cambios clave:**
- **Eliminado**: "Tasks" module (usado para UE::Tasks)
- **Eliminado**: Otros módulos relacionados con threading
- **Mantenido**: Solo módulos esenciales para funcionalidad básica

```csharp
using UnrealBuildTool;

public class AsyncMessageSystem : ModuleRules
{
    public AsyncMessageSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",           // Funcionalidad básica de UE
            "CoreUObject",    // Sistema de objetos de UE
            "Engine",         // Funcionalidad del engine
            "StructUtils"     // Para FInstancedStruct (payloads)
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "DeveloperSettings"  // Para configuraciones del plugin
        });

        // IMPORTANTE: Removimos módulos relacionados con threading:
        // - "Tasks" (para UE::Tasks)
        // - Cualquier otro módulo de threading
        // Esto previene el uso accidental de APIs multi-thread
    }
}
````

**1.3 Crear Archivos Básicos del Módulo**

**¿Por qué necesitamos estos archivos?**
Todo plugin de Unreal Engine necesita una clase de módulo que maneje la inicialización y limpieza. En nuestro caso, es muy simple porque no tenemos que inicializar sistemas de threading.

**Diferencias con el sistema original:**

- **Simplificado**: No inicializamos pools de threads ni sistemas asíncronos
- **Directo**: Solo logging básico para debugging
- **Sin cleanup complejo**: No hay que limpiar recursos de threading

Crear `Private/AsyncMessageSystemModule.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Módulo simple sin inicialización de threading
class FAsyncMessageSystemModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    // NOTA: En el sistema original, aquí habría inicialización de:
    // - Thread pools
    // - Task systems
    // - Atomic counters globales
    // Todo eso se elimina en la versión single-thread
};
```

Crear `Private/AsyncMessageSystemModule.cpp`:

```cpp
#include "AsyncMessageSystemModule.h"
#include "AsyncMessageSystemLogs.h"

#define LOCTEXT_NAMESPACE "FAsyncMessageSystemModule"

void FAsyncMessageSystemModule::StartupModule()
{
    // Inicialización simple - solo logging
    // En el sistema original aquí se inicializarían:
    // - Sistemas de threading
    // - Pools de workers
    // - Contadores atómicos globales
    UE_LOG(LogAsyncMessageSystem, Log, TEXT("AsyncMessageSystem module started (Single-Thread)"));
}

void FAsyncMessageSystemModule::ShutdownModule()
{
    // Limpieza simple - solo logging
    // En el sistema original aquí se limpiarían:
    // - Threads activos
    // - Colas thread-safe
    // - Recursos de sincronización
    UE_LOG(LogAsyncMessageSystem, Log, TEXT("AsyncMessageSystem module shutdown"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAsyncMessageSystemModule, AsyncMessageSystem)
```

## Fase 2: Componentes del Sistema Central

### Paso 2: Implementar Sistema de Logging

**¿Por qué empezamos con logging?**
El logging es crucial para debugging, especialmente cuando migramos de un sistema complejo. Nos ayudará a entender el flujo de ejecución y detectar problemas.

**Diferencias con el sistema original:**

- **Simplificado**: No necesitamos logging thread-safe
- **Más información**: Agregamos contexto de función y línea para debugging
- **Sin overhead**: No hay locks en el logging

**2.1 Crear Public/AsyncMessageSystemLogs.h**

**¿Por qué estos macros?**
Los macros nos dan información de contexto (función y línea) que es invaluable para debugging. En un sistema single-thread, podemos rastrear fácilmente el flujo de ejecución.

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

// Categoría de log para nuestro sistema
DECLARE_LOG_CATEGORY_EXTERN(LogAsyncMessageSystem, Log, All);

// Macros de logging con contexto automático
// VENTAJA SINGLE-THREAD: No necesitamos logging thread-safe
#define ASYNC_MESSAGE_ERROR(Format, ...) \
    UE_LOG(LogAsyncMessageSystem, Error, TEXT("[%hs:%d] " Format), __func__, __LINE__, ##__VA_ARGS__)

#define ASYNC_MESSAGE_WARNING(Format, ...) \
    UE_LOG(LogAsyncMessageSystem, Warning, TEXT("[%hs:%d] " Format), __func__, __LINE__, ##__VA_ARGS__)

#define ASYNC_MESSAGE_LOG(Format, ...) \
    UE_LOG(LogAsyncMessageSystem, Log, TEXT("[%hs:%d] " Format), __func__, __LINE__, ##__VA_ARGS__)

// NOTA: En el sistema original, estos macros tendrían que ser thread-safe
// y posiblemente usar locks para evitar que los logs se mezclen entre threads
```

**2.2 Crear Private/AsyncMessageSystemLogs.cpp**

```cpp
#include "AsyncMessageSystemLogs.h"

// Definición simple de la categoría de log
// En single-thread no necesitamos inicialización especial
DEFINE_LOG_CATEGORY(LogAsyncMessageSystem);
```

### Paso 3: Implementar Sistema de ID de Mensajes

**¿Por qué necesitamos IDs de mensajes?**
Los IDs de mensajes son la forma de identificar tipos de mensajes en el sistema. Son como "canales" o "topics" en otros sistemas de mensajería.

**Diferencias clave con el sistema original:**

- **Mismo diseño**: Los IDs no necesitan cambios para single-thread
- **Sin overhead**: No hay sincronización necesaria para comparaciones
- **Simplificado**: Usamos FName que ya es eficiente en UE

**3.1 Crear Public/AsyncMessageId.h**

**¿Por qué usar FName?**
FName en Unreal Engine es muy eficiente para comparaciones y hashing. Es perfecto para identificadores que se comparan frecuentemente.

````cpp
#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "AsyncMessageId.generated.h"

USTRUCT(BlueprintType)
struct ASYNCMESSAGESYSTEM_API FAsyncMessageId
{
    GENERATED_BODY()

public:
    FAsyncMessageId() = default;
    explicit FAsyncMessageId(const FName& InName) : Name(InName) {}
    explicit FAsyncMessageId(const FString& InString) : Name(*InString) {}

    // Validación simple
    bool IsValid() const { return !Name.IsNone(); }

    // Operadores de comparación
    // VENTAJA SINGLE-THREAD: No necesitamos operaciones atómicas aquí
    bool operator==(const FAsyncMessageId& Other) const { return Name == Other.Name; }
    bool operator!=(const FAsyncMessageId& Other) const { return Name != Other.Name; }
    bool operator<(const FAsyncMessageId& Other) const { return Name.LexicalLess(Other.Name); }

    // Conversión a string para debugging
    FString ToString() const { return Name.ToString(); }
    FName ToName() const { return Name; }

    // Función hash para usar en containers (TMap, TSet)
    // IMPORTANTE: En single-thread podemos usar containers normales
    friend uint32 GetTypeHash(const FAsyncMessageId& MessageId)
    {
        return GetTypeHash(MessageId.Name);
    }

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FName Name;

    // NOTA: En el sistema original, podría haber campos adicionales para
    // tracking de threads, pero aquí no los necesitamos
};
```**3
.2 Create Private/AsyncMessageId.cpp**
```cpp
#include "AsyncMessageId.h"

// Implementation is mostly in header due to simple nature
// Add any complex methods here if needed in the future
````

### Paso 4: Implementar Sistema de Handles de Mensajes

**¿Qué son los handles?**
Los handles son identificadores únicos para las suscripciones a mensajes. Permiten desuscribirse de mensajes específicos más tarde.

**DIFERENCIA CRÍTICA con el sistema original:**

- **Original**: Usaba `std::atomic<uint32>` para generar IDs thread-safe
- **Nuevo**: Usa `uint32` simple con incremento normal
- **¿Por qué funciona?**: En single-thread no hay condiciones de carrera

**4.1 Crear Public/AsyncMessageHandle.h**

**¿Por qué este cambio es seguro?**
En el sistema original, múltiples threads podían generar handles simultáneamente, requiriendo operaciones atómicas. En single-thread, solo un hilo genera handles, eliminando la necesidad de sincronización.

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AsyncMessageHandle.generated.h"

// Forward declarations
class FAsyncMessageBindingEndpoint;

USTRUCT(BlueprintType)
struct ASYNCMESSAGESYSTEM_API FAsyncMessageHandle
{
    GENERATED_BODY()

public:
    FAsyncMessageHandle() : HandleId(0) {}

    // CAMBIO CRÍTICO: Generación de handles sin atomics
    // Original: std::atomic<uint32> NextHandleId
    // Nuevo: uint32 NextHandleId (simple counter)
    static FAsyncMessageHandle GenerateNewHandle();

    // Validación simple
    bool IsValid() const { return HandleId != 0; }

    // Operadores de comparación
    bool operator==(const FAsyncMessageHandle& Other) const { return HandleId == Other.HandleId; }
    bool operator!=(const FAsyncMessageHandle& Other) const { return HandleId != Other.HandleId; }
    bool operator<(const FAsyncMessageHandle& Other) const { return HandleId < Other.HandleId; }

    // Hash function para containers
    friend uint32 GetTypeHash(const FAsyncMessageHandle& Handle)
    {
        return GetTypeHash(Handle.HandleId);
    }

    // Obtener endpoint asociado (si existe)
    TSharedPtr<FAsyncMessageBindingEndpoint> GetBindingEndpoint() const;

    // Conversión a string para debugging
    FString ToString() const { return FString::Printf(TEXT("Handle_%u"), HandleId); }

private:
    explicit FAsyncMessageHandle(uint32 InHandleId) : HandleId(InHandleId) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    uint32 HandleId;

    // CAMBIO PRINCIPAL: Counter simple en lugar de atomic
    // ¿Por qué es seguro? Solo un thread (game thread) genera handles
    static uint32 NextHandleId; // Era: std::atomic<uint32> NextHandleId;

    friend class FAsyncMessageBindingEndpoint;
};
```

**4.2 Crear Private/AsyncMessageHandle.cpp**

**¿Por qué esta implementación es más simple?**
La generación de handles es dramáticamente más simple sin threading. No necesitamos operaciones atómicas costosas.

```cpp
#include "AsyncMessageHandle.h"
#include "AsyncMessageBindingEndpoint.h"

// Inicializar contador estático
// CAMBIO: Era std::atomic<uint32> NextHandleId{1};
// Ahora: uint32 simple
uint32 FAsyncMessageHandle::NextHandleId = 1;

FAsyncMessageHandle FAsyncMessageHandle::GenerateNewHandle()
{
    // DIFERENCIA CRÍTICA: Incremento simple vs operación atómica
    // Original: return FAsyncMessageHandle(NextHandleId.fetch_add(1));
    // Nuevo: Simple increment - no atomic needed

    // ¿Por qué es seguro? Solo el game thread llama esta función
    // No hay condiciones de carrera posibles
    return FAsyncMessageHandle(NextHandleId++);
}

TSharedPtr<FAsyncMessageBindingEndpoint> FAsyncMessageHandle::GetBindingEndpoint() const
{
    // Implementación será completada cuando implementemos FAsyncMessageBindingEndpoint
    // Por ahora, retornar nullptr
    return nullptr;
}
```

### Step 5: Implement Binding Options

**5.1 Create Public/AsyncMessageBindingOptions.h**

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "AsyncMessageBindingOptions.generated.h"

// Simplified enum - only tick group option
UENUM(BlueprintType)
enum class EAsyncMessageBindingOptions : uint8
{
    UseTickGroup = 0    // Only option - process on game thread
};

USTRUCT(BlueprintType)
struct ASYNCMESSAGESYSTEM_API FAsyncMessageBindingOptions
{
    GENERATED_BODY()

public:
    // Default constructor
    FAsyncMessageBindingOptions(ETickingGroup InTickGroup = TG_DuringPhysics)
        : BindingOption(EAsyncMessageBindingOptions::UseTickGroup)
        , TickGroup(InTickGroup)
    {}

    // Getters
    EAsyncMessageBindingOptions GetBindingOption() const { return BindingOption; }
    ETickingGroup GetTickGroup() const { return TickGroup; }

    // Comparison operators for use in containers
    bool operator==(const FAsyncMessageBindingOptions& Other) const
    {
        return BindingOption == Other.BindingOption && TickGroup == Other.TickGroup;
    }

    bool operator!=(const FAsyncMessageBindingOptions& Other) const
    {
        return !(*this == Other);
    }

    // Hash function for use in containers
    friend uint32 GetTypeHash(const FAsyncMessageBindingOptions& Options)
    {
        return HashCombine(GetTypeHash(Options.BindingOption), GetTypeHash(Options.TickGroup));
    }

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EAsyncMessageBindingOptions BindingOption;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    ETickingGroup TickGroup;
};
```

**5.2 Create Private/AsyncMessageBindingOptions.cpp**

````cpp
#include "AsyncMessageBindingOptions.h"

// Implementation is mostly in header due to simple nature
// Add any complex methods here if needed in the future
```## Ph
ase 3: Message Structure and Storage

### Step 6: Implement Core Message Structure

**6.1 Create Public/AsyncMessage.h**
```cpp
#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "AsyncMessageId.h"
#include "AsyncMessage.generated.h"

// Forward declarations
class FAsyncMessageBindingEndpoint;

USTRUCT(BlueprintType)
struct ASYNCMESSAGESYSTEM_API FAsyncMessage
{
    GENERATED_BODY()

public:
    FAsyncMessage() : SequenceId(0), Timestamp(0.0) {}

    // Constructor simplified - no thread capture
    FAsyncMessage(const FAsyncMessageId& InMessageId,
                  const FInstancedStruct& InPayload,
                  TWeakPtr<FAsyncMessageBindingEndpoint> InEndpoint = nullptr);

    // Getters
    const FAsyncMessageId& GetMessageId() const { return MessageId; }
    const FInstancedStruct& GetPayload() const { return Payload; }
    TWeakPtr<FAsyncMessageBindingEndpoint> GetTargetEndpoint() const { return TargetEndpoint; }
    uint32 GetSequenceId() const { return SequenceId; }
    double GetTimestamp() const { return Timestamp; }

    // Validation
    bool IsValid() const { return MessageId.IsValid() && Payload.IsValid(); }

    // String conversion for debugging
    FString ToString() const;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FAsyncMessageId MessageId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FInstancedStruct Payload;

    // Target endpoint (weak pointer to avoid circular references)
    TWeakPtr<FAsyncMessageBindingEndpoint> TargetEndpoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    uint32 SequenceId;  // Simple counter, no atomic needed

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    double Timestamp;   // For debugging/profiling

    static uint32 NextSequenceId; // Simple counter for sequence IDs
};
````

**6.2 Create Private/AsyncMessage.cpp**

```cpp
#include "AsyncMessage.h"
#include "AsyncMessageBindingEndpoint.h"

// Initialize static counter
uint32 FAsyncMessage::NextSequenceId = 1;

FAsyncMessage::FAsyncMessage(const FAsyncMessageId& InMessageId,
                             const FInstancedStruct& InPayload,
                             TWeakPtr<FAsyncMessageBindingEndpoint> InEndpoint)
    : MessageId(InMessageId)
    , Payload(InPayload)
    , TargetEndpoint(InEndpoint)
    , SequenceId(NextSequenceId++)
    , Timestamp(FPlatformTime::Seconds())
{
}

FString FAsyncMessage::ToString() const
{
    return FString::Printf(TEXT("Message[%s] Seq[%u] Time[%.3f]"),
                          *MessageId.ToString(),
                          SequenceId,
                          Timestamp);
}
```

### Step 7: Implement Message Storage System

**7.1 Create Public/AsyncMessageStore.h**

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AsyncMessage.h"
#include "AsyncMessageId.h"

// Message storage and retrieval system (single-threaded)
class ASYNCMESSAGESYSTEM_API FAsyncMessageStore
{
public:
    FAsyncMessageStore() = default;
    ~FAsyncMessageStore() = default;

    // Store a message
    void StoreMessage(const FAsyncMessage& Message);

    // Retrieve messages by ID
    TArray<FAsyncMessage> RetrieveMessages(const FAsyncMessageId& MessageId);

    // Clear messages by ID
    void ClearMessages(const FAsyncMessageId& MessageId);

    // Clear all messages
    void ClearAllMessages();

    // Get message count for debugging
    int32 GetMessageCount() const;
    int32 GetMessageCount(const FAsyncMessageId& MessageId) const;

private:
    // Use standard containers - no thread safety needed
    TMap<FAsyncMessageId, TArray<FAsyncMessage>> StoredMessages;

    // Note: Removed FCriticalSection StorageCS - not needed in single-threaded system
};
```

**7.2 Create Private/AsyncMessageStore.cpp**

```cpp
#include "AsyncMessageStore.h"
#include "AsyncMessageSystemLogs.h"

void FAsyncMessageStore::StoreMessage(const FAsyncMessage& Message)
{
    if (!Message.IsValid())
    {
        ASYNC_MESSAGE_WARNING("Attempted to store invalid message");
        return;
    }

    // Simple array addition - no locking needed
    StoredMessages.FindOrAdd(Message.GetMessageId()).Add(Message);

    ASYNC_MESSAGE_LOG("Stored message: %s", *Message.ToString());
}

TArray<FAsyncMessage> FAsyncMessageStore::RetrieveMessages(const FAsyncMessageId& MessageId)
{
    if (TArray<FAsyncMessage>* Messages = StoredMessages.Find(MessageId))
    {
        return *Messages;
    }

    return TArray<FAsyncMessage>();
}

void FAsyncMessageStore::ClearMessages(const FAsyncMessageId& MessageId)
{
    if (StoredMessages.Remove(MessageId) > 0)
    {
        ASYNC_MESSAGE_LOG("Cleared messages for ID: %s", *MessageId.ToString());
    }
}

void FAsyncMessageStore::ClearAllMessages()
{
    int32 TotalMessages = GetMessageCount();
    StoredMessages.Empty();

    ASYNC_MESSAGE_LOG("Cleared all messages (Total: %d)", TotalMessages);
}

int32 FAsyncMessageStore::GetMessageCount() const
{
    int32 Count = 0;
    for (const auto& Pair : StoredMessages)
    {
        Count += Pair.Value.Num();
    }
    return Count;
}

int32 FAsyncMessageStore::GetMessageCount(const FAsyncMessageId& MessageId) const
{
    if (const TArray<FAsyncMessage>* Messages = StoredMessages.Find(MessageId))
    {
        return Messages->Num();
    }
    return 0;
}
```

## Fase 4: Sistema de Binding y Endpoints

### Paso 8: Implementar Message Binding Endpoint

**¿Qué es un Binding Endpoint?**
Un endpoint es un objeto que puede recibir mensajes. Maneja las suscripciones y ejecuta los callbacks cuando llegan mensajes.

**DIFERENCIAS CRÍTICAS con el sistema original:**

- **Sin locks**: No necesitamos `FCriticalSection` para proteger datos
- **Containers simples**: Usamos `TMap` y `TArray` normales en lugar de thread-safe
- **Shared pointers**: `ESPMode::NotThreadSafe` en lugar de `ThreadSafe`
- **Procesamiento directo**: Los callbacks se ejecutan inmediatamente, sin queuing entre threads

**8.1 Create Public/AsyncMessageBindingEndpoint.h**

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AsyncMessageId.h"
#include "AsyncMessageHandle.h"
#include "AsyncMessageBindingOptions.h"

// Forward declarations
struct FAsyncMessage;

// Callback function type for message listeners
DECLARE_DELEGATE_OneParam(FMessageCallbackFunc, const FAsyncMessage&);

// Data structure for bound listeners
struct FBoundListener
{
    FAsyncMessageHandle Handle;
    FMessageCallbackFunc Callback;
    TWeakPtr<FAsyncMessageBindingEndpoint> Endpoint;

    FBoundListener() = default;
    FBoundListener(const FAsyncMessageHandle& InHandle,
                   const FMessageCallbackFunc& InCallback,
                   TWeakPtr<FAsyncMessageBindingEndpoint> InEndpoint)
        : Handle(InHandle), Callback(InCallback), Endpoint(InEndpoint) {}
};

// Data structure for message binding information
struct FAsyncMessageBoundData
{
    // Map of binding options to listeners
    TMap<FAsyncMessageBindingOptions, TArray<FBoundListener>> ListenerMap;

    // Add a listener
    void AddListener(const FAsyncMessageBindingOptions& Options, const FBoundListener& Listener)
    {
        ListenerMap.FindOrAdd(Options).Add(Listener);
    }

    // Remove a listener by handle
    bool RemoveListener(const FAsyncMessageHandle& Handle)
    {
        for (auto& Pair : ListenerMap)
        {
            TArray<FBoundListener>& Listeners = Pair.Value;
            for (int32 i = Listeners.Num() - 1; i >= 0; --i)
            {
                if (Listeners[i].Handle == Handle)
                {
                    Listeners.RemoveAt(i);
                    return true;
                }
            }
        }
        return false;
    }

    // Get total listener count
    int32 GetListenerCount() const
    {
        int32 Count = 0;
        for (const auto& Pair : ListenerMap)
        {
            Count += Pair.Value.Num();
        }
        return Count;
    }
};

// Interface for objects that can receive messages
class IAsyncMessageBindingEndpointInterface
{
public:
    virtual ~IAsyncMessageBindingEndpointInterface() = default;
    virtual TWeakPtr<FAsyncMessageBindingEndpoint> GetEndpoint() = 0;
};

// Message binding endpoint - manages listeners for messages
// CAMBIO CRÍTICO: ESPMode::NotThreadSafe en lugar de ThreadSafe
class ASYNCMESSAGESYSTEM_API FAsyncMessageBindingEndpoint : public TSharedFromThis<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe>
{
public:
    FAsyncMessageBindingEndpoint() = default;
    virtual ~FAsyncMessageBindingEndpoint() = default;

    // Encontrar o agregar datos de mensaje para un ID específico
    FAsyncMessageBoundData& FindOrAddMessageData(const FAsyncMessageId& MessageId);

    // Procesar un mensaje (llamar todos los listeners)
    // DIFERENCIA: Ejecución directa, sin queuing entre threads
    void ProcessMessage(const FAsyncMessage& Message);

    // Agregar un listener para un mensaje específico
    FAsyncMessageHandle AddListener(const FAsyncMessageId& MessageId,
                                   const FAsyncMessageBindingOptions& Options,
                                   const FMessageCallbackFunc& Callback);

    // Remover un listener por handle
    bool RemoveListener(const FAsyncMessageHandle& Handle);

    // Obtener conteo de listeners para debugging
    int32 GetListenerCount() const;
    int32 GetListenerCount(const FAsyncMessageId& MessageId) const;

private:
    // Map de IDs de mensaje a sus datos bound
    // CAMBIO IMPORTANTE: TMap normal en lugar de thread-safe container
    TMap<FAsyncMessageId, FAsyncMessageBoundData> BoundMessageData;

    // ELIMINADO: FCriticalSection BoundMessageDataCS;
    // ¿Por qué? En single-thread no necesitamos proteger el acceso a datos
    // El sistema original tenía: mutable FCriticalSection BoundMessageDataCS;
};
```

**8.2 Create Private/AsyncMessageBindingEndpoint.cpp**

```cpp
#include "AsyncMessageBindingEndpoint.h"
#include "AsyncMessage.h"
#include "AsyncMessageSystemLogs.h"

FAsyncMessageBoundData& FAsyncMessageBindingEndpoint::FindOrAddMessageData(const FAsyncMessageId& MessageId)
{
    return BoundMessageData.FindOrAdd(MessageId);
}

void FAsyncMessageBindingEndpoint::ProcessMessage(const FAsyncMessage& Message)
{
    if (!Message.IsValid())
    {
        ASYNC_MESSAGE_WARNING("Attempted to process invalid message");
        return;
    }

    FAsyncMessageBoundData* MessageData = BoundMessageData.Find(Message.GetMessageId());
    if (!MessageData)
    {
        // No listeners for this message type
        return;
    }

    // Process all listeners for this message
    for (const auto& OptionsPair : MessageData->ListenerMap)
    {
        const TArray<FBoundListener>& Listeners = OptionsPair.Value;

        for (const FBoundListener& Listener : Listeners)
        {
            if (Listener.Callback.IsBound())
            {
                try
                {
                    Listener.Callback.Execute(Message);
                }
                catch (...)
                {
                    ASYNC_MESSAGE_ERROR("Exception occurred while executing message callback for message: %s",
                                       *Message.ToString());
                }
            }
        }
    }
}

FAsyncMessageHandle FAsyncMessageBindingEndpoint::AddListener(const FAsyncMessageId& MessageId,
                                                             const FAsyncMessageBindingOptions& Options,
                                                             const FMessageCallbackFunc& Callback)
{
    if (!MessageId.IsValid())
    {
        ASYNC_MESSAGE_ERROR("Cannot add listener for invalid message ID");
        return FAsyncMessageHandle();
    }

    if (!Callback.IsBound())
    {
        ASYNC_MESSAGE_ERROR("Cannot add unbound callback for message: %s", *MessageId.ToString());
        return FAsyncMessageHandle();
    }

    FAsyncMessageHandle Handle = FAsyncMessageHandle::GenerateNewHandle();
    FBoundListener Listener(Handle, Callback, AsShared());

    FAsyncMessageBoundData& MessageData = FindOrAddMessageData(MessageId);
    MessageData.AddListener(Options, Listener);

    ASYNC_MESSAGE_LOG("Added listener %s for message: %s", *Handle.ToString(), *MessageId.ToString());

    return Handle;
}

bool FAsyncMessageBindingEndpoint::RemoveListener(const FAsyncMessageHandle& Handle)
{
    if (!Handle.IsValid())
    {
        ASYNC_MESSAGE_WARNING("Cannot remove listener with invalid handle");
        return false;
    }

    // Search through all message data to find and remove the listener
    for (auto& Pair : BoundMessageData)
    {
        if (Pair.Value.RemoveListener(Handle))
        {
            ASYNC_MESSAGE_LOG("Removed listener %s for message: %s",
                             *Handle.ToString(), *Pair.Key.ToString());
            return true;
        }
    }

    ASYNC_MESSAGE_WARNING("Could not find listener with handle: %s", *Handle.ToString());
    return false;
}

int32 FAsyncMessageBindingEndpoint::GetListenerCount() const
{
    int32 Count = 0;
    for (const auto& Pair : BoundMessageData)
    {
        Count += Pair.Value.GetListenerCount();
    }
    return Count;
}

int32 FAsyncMessageBindingEndpoint::GetListenerCount(const FAsyncMessageId& MessageId) const
{
    if (const FAsyncMessageBoundData* MessageData = BoundMessageData.Find(MessageId))
    {
        return MessageData->GetListenerCount();
    }
    return 0;
}
```

## Fase 5: Sistema Central de Mensajes

### Paso 9: Implementar la Base del Sistema Central de Mensajes

**¿Por qué este es el componente más crítico?**
Este es el corazón del sistema. Aquí es donde ocurren los cambios más dramáticos de la migración multi-thread a single-thread.

**TRANSFORMACIONES PRINCIPALES:**

1. **TMpscQueue → TArray**: Colas thread-safe reemplazadas por arrays simples
2. **FCriticalSection eliminados**: Sin locks ni critical sections
3. **Procesamiento en tick**: Todo ocurre durante el tick del game thread
4. **Sin async tasks**: Eliminamos UE::Tasks y ENamedThreads completamente

**9.1 Crear Public/AsyncMessageSystemBase.h**

**¿Por qué este header es tan diferente?**
Este archivo define la clase central del sistema. Los cambios aquí representan la filosofía completa de la migración single-thread.

**Estructuras de Datos Clave:**

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AsyncMessage.h"
#include "AsyncMessageId.h"
#include "AsyncMessageHandle.h"
#include "AsyncMessageBindingOptions.h"
#include "AsyncMessageBindingEndpoint.h"
#include "AsyncMessageStore.h"

// Queued message structure for processing
struct FQueuedMessage
{
    FAsyncMessage Message;
    TWeakPtr<FAsyncMessageBindingEndpoint> TargetEndpoint;

    FQueuedMessage() = default;
    FQueuedMessage(const FAsyncMessage& InMessage, TWeakPtr<FAsyncMessageBindingEndpoint> InEndpoint = nullptr)
        : Message(InMessage), TargetEndpoint(InEndpoint) {}
};

// Pending listener binding structure
template<typename CallbackType>
struct FPendingBoundListener
{
    FAsyncMessageId MessageId;
    FAsyncMessageBindingOptions Options;

    struct FListenerData
    {
        FAsyncMessageHandle Handle;
        CallbackType Callback;
        TWeakPtr<FAsyncMessageBindingEndpoint> Endpoint;
    } Data;

    FPendingBoundListener() = default;
    FPendingBoundListener(const FAsyncMessageId& InMessageId,
                         const FAsyncMessageBindingOptions& InOptions,
                         const FAsyncMessageHandle& InHandle,
                         const CallbackType& InCallback,
                         TWeakPtr<FAsyncMessageBindingEndpoint> InEndpoint)
        : MessageId(InMessageId), Options(InOptions)
    {
        Data.Handle = InHandle;
        Data.Callback = InCallback;
        Data.Endpoint = InEndpoint;
    }
};

// CLASE PRINCIPAL: Sistema de mensajes single-threaded
// CAMBIO FUNDAMENTAL: ESPMode::NotThreadSafe
class ASYNCMESSAGESYSTEM_API FAsyncMessageSystemBase : public TSharedFromThis<FAsyncMessageSystemBase, ESPMode::NotThreadSafe>
{
public:
    FAsyncMessageSystemBase();
    virtual ~FAsyncMessageSystemBase();

    // MÉTODO CENTRAL: Procesamiento en tick - punto único de entrada
    // DIFERENCIA: En el sistema original había múltiples puntos de entrada desde diferentes threads
    virtual void ExecuteTick(float DeltaTime, ETickingGroup TickGroup);

    // Encolar un mensaje para broadcast
    // CAMBIO: Encolado simple en TArray, no en cola thread-safe
    bool QueueMessageForBroadcast(const FAsyncMessageId& MessageId,
                                  const FInstancedStruct& Payload,
                                  TWeakPtr<FAsyncMessageBindingEndpoint> TargetEndpoint = nullptr);

    // Vincular un listener a un mensaje
    // DIFERENCIA: No necesita locks para agregar a la cola de pending listeners
    FAsyncMessageHandle BindListener(const FAsyncMessageId& MessageId,
                                    const FAsyncMessageBindingOptions& Options,
                                    const FMessageCallbackFunc& Callback,
                                    TWeakPtr<FAsyncMessageBindingEndpoint> Endpoint);

    // Desvincular un listener
    // CAMBIO: Encolado simple para procesamiento en próximo tick
    void UnbindListener(const FAsyncMessageHandle& Handle);

    // Get message store for advanced operations
    FAsyncMessageStore& GetMessageStore() { return MessageStore; }
    const FAsyncMessageStore& GetMessageStore() const { return MessageStore; }

    // Debug information
    int32 GetQueuedMessageCount() const { return MessageQueue.Num(); }
    int32 GetPendingListenerCount() const { return PendingBoundListenerQueue.Num(); }
    int32 GetUnbindRequestCount() const { return UnbindHandleRequestQueue.Num(); }

protected:
    // Processing methods (simplified - no locking)
    virtual void ProcessMessageQueueForBinding_Impl();
    virtual void ProcessUnbindHandleRequests();
    virtual void ProcessListenersPendingBinding();

private:
    // CAMBIO CRÍTICO 1: Arrays simples en lugar de colas thread-safe
    // Original: TMpscQueue<FQueuedMessage> MessageQueue;
    // Nuevo: TArray<FQueuedMessage> MessageQueue;
    TArray<FQueuedMessage> MessageQueue;
    TArray<FPendingBoundListener<FMessageCallbackFunc>> PendingBoundListenerQueue;
    TArray<FAsyncMessageHandle> UnbindHandleRequestQueue;

    // Almacenamiento de mensajes
    FAsyncMessageStore MessageStore;

    // CAMBIO CRÍTICO 2: ESPMode::NotThreadSafe
    // Map de listeners bound por mensaje
    TMap<FAsyncMessageId, TArray<TSharedPtr<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe>>> BoundMessageListenerMap;

    // ELIMINADO COMPLETAMENTE: Todos los FCriticalSection y FScopeLock
    // El sistema original tenía:
    // - FCriticalSection MessageListenerMapCS;
    // - FCriticalSection MessageQueueCS;
    // - FCriticalSection PendingListenerCS;
    // ¿Por qué los eliminamos? En single-thread no hay acceso concurrente
};
```

**9.2 Crear Private/AsyncMessageSystemBase.cpp**

**¿Por qué esta implementación es más simple?**
La implementación single-thread elimina toda la complejidad de sincronización. Cada método es más directo y predecible.

**Flujo de Procesamiento:**

1. **ExecuteTick()**: Procesa todo en orden secuencial
2. **QueueMessageForBroadcast()**: Simple adición a array
3. **BindListener()**: Encolado para procesamiento posterior
4. **Métodos de procesamiento**: Sin locks, acceso directo a datos

````cpp
#include "AsyncMessageSystemBase.h"
#include "AsyncMessageSystemLogs.h"

FAsyncMessageSystemBase::FAsyncMessageSystemBase()
{
    ASYNC_MESSAGE_LOG("AsyncMessageSystemBase created");
}

FAsyncMessageSystemBase::~FAsyncMessageSystemBase()
{
    ASYNC_MESSAGE_LOG("AsyncMessageSystemBase destroyed");
}

void FAsyncMessageSystemBase::ExecuteTick(float DeltaTime, ETickingGroup TickGroup)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(FAsyncMessageSystemBase::ExecuteTick);

    // FLUJO SECUENCIAL: Procesamos todo en orden específico
    // DIFERENCIA CRÍTICA: En el sistema original, estos procesos ocurrían
    // en diferentes threads y requerían sincronización compleja

    // 1. Primero procesamos nuevos listeners
    ProcessListenersPendingBinding();

    // 2. Luego procesamos desvinculaciones
    ProcessUnbindHandleRequests();

    // 3. Finalmente procesamos mensajes
    ProcessMessageQueueForBinding_Impl();

    // ¿Por qué este orden? Garantiza que:
    // - Los listeners estén listos antes de procesar mensajes
    // - Las desvinculaciones se procesen antes de enviar mensajes
    // - Todo sea determinístico y predecible
}

bool FAsyncMessageSystemBase::QueueMessageForBroadcast(const FAsyncMessageId& MessageId,
                                                       const FInstancedStruct& Payload,
                                                       TWeakPtr<FAsyncMessageBindingEndpoint> TargetEndpoint)
{
    // Validación básica
    if (!MessageId.IsValid())
    {
        ASYNC_MESSAGE_ERROR("Invalid MessageId provided for broadcast");
        return false;
    }

    if (!Payload.IsValid())
    {
        ASYNC_MESSAGE_ERROR("Invalid Payload provided for message '%s'", *MessageId.ToString());
        return false;
    }

    // DIFERENCIA CRÍTICA: Encolado simple sin locks
    // Original: MessageQueue.Enqueue(Message); // Con locks internos
    // Nuevo: MessageQueue.Add(Message); // Simple adición a array

    FAsyncMessage Message(MessageId, Payload, TargetEndpoint);
    MessageQueue.Add(FQueuedMessage(Message, TargetEndpoint));

    ASYNC_MESSAGE_LOG("Queued message for broadcast: %s", *Message.ToString());
    return true;

    // ¿Por qué es seguro? Solo el game thread llama esta función
    // No hay condiciones de carrera posibles
}

FAsyncMessageHandle FAsyncMessageSystemBase::BindListener(const FAsyncMessageId& MessageId,
                                                         const FAsyncMessageBindingOptions& Options,
                                                         const FMessageCallbackFunc& Callback,
                                                         TWeakPtr<FAsyncMessageBindingEndpoint> Endpoint)
{
    if (!MessageId.IsValid())
    {
        ASYNC_MESSAGE_ERROR("Cannot bind listener for invalid message ID");
        return FAsyncMessageHandle();
    }

    if (!Callback.IsBound())
    {
        ASYNC_MESSAGE_ERROR("Cannot bind invalid callback for message: %s", *MessageId.ToString());
        return FAsyncMessageHandle();
    }

    FAsyncMessageHandle Handle = FAsyncMessageHandle::GenerateNewHandle();

    // Queue the binding for processing on next tick
    PendingBoundListenerQueue.Add(FPendingBoundListener<FMessageCallbackFunc>(
        MessageId, Options, Handle, Callback, Endpoint));

    ASYNC_MESSAGE_LOG("Queued listener binding %s for message: %s", *Handle.ToString(), *MessageId.ToString());
    return Handle;
}

void FAsyncMessageSystemBase::UnbindListener(const FAsyncMessageHandle& Handle)
{
    if (!Handle.IsValid())
    {
        ASYNC_MESSAGE_WARNING("Cannot unbind invalid handle");
        return;
    }

    // Queue the unbind request for processing
    UnbindHandleRequestQueue.Add(Handle);

    ASYNC_MESSAGE_LOG("Queued unbind request for handle: %s", *Handle.ToString());
}

void FAsyncMessageSystemBase::ProcessMessageQueueForBinding_Impl()
{
    TRACE_CPUPROFILER_EVENT_SCOPE(FAsyncMessageSystemBase::ProcessMessageQueueForBinding_Impl);

    if (MessageQueue.IsEmpty())
    {
        return;
    }

    // Process all queued messages
    for (const FQueuedMessage& QueuedMessage : MessageQueue)
    {
        const FAsyncMessage& Message = QueuedMessage.Message;

        // Store the message
        MessageStore.StoreMessage(Message);

        // Find all endpoints that should receive this message
        if (TArray<TSharedPtr<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe>>* Endpoints =
            BoundMessageListenerMap.Find(Message.GetMessageId()))
        {
            for (TSharedPtr<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe> Endpoint : *Endpoints)
            {
                if (Endpoint.IsValid())
                {
                    Endpoint->ProcessMessage(Message);
                }
            }
        }
    }

    // Clear processed messages
    MessageQueue.Empty();
}

void FAsyncMessageSystemBase::ProcessUnbindHandleRequests()
{
    TRACE_CPUPROFILER_EVENT_SCOPE(FAsyncMessageSystemBase::ProcessUnbindHandleRequests);

    if (UnbindHandleRequestQueue.IsEmpty())
    {
        return;
    }

    // Process all unbind requests
    for (const FAsyncMessageHandle& Handle : UnbindHandleRequestQueue)
    {
        // Search through all endpoints to find and remove the listener
        bool bFound = false;
        for (auto& Pair : BoundMessageListenerMap)
        {
            TArray<TSharedPtr<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe>>& Endpoints = Pair.Value;

            for (TSharedPtr<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe> Endpoint : Endpoints)
            {
                if (Endpoint.IsValid() && Endpoint->RemoveListener(Handle))
                {
                    bFound = true;
                    break;
                }
            }

            if (bFound) break;
        }

        if (!bFound)
        {
            ASYNC_MESSAGE_WARNING("Could not find listener to unbind with handle: %s", *Handle.ToString());
        }
    }

    // Clear processed requests
    UnbindHandleRequestQueue.Empty();
}

void FAsyncMessageSystemBase::ProcessListenersPendingBinding()
{
    TRACE_CPUPROFILER_EVENT_SCOPE(FAsyncMessageSystemBase::ProcessListenersPendingBinding);

    if (PendingBoundListenerQueue.IsEmpty())
    {
        return;
    }

    // Process all pending listener bindings
    for (const FPendingBoundListener<FMessageCallbackFunc>& PendingListener : PendingBoundListenerQueue)
    {
        TSharedPtr<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe> Endpoint = PendingListener.Data.Endpoint.Pin();

        if (Endpoint.IsValid())
        {
            // Add the listener to the endpoint
            Endpoint->AddListener(PendingListener.MessageId, PendingListener.Options, PendingListener.Data.Callback);

            // Add endpoint to our bound listeners map
            BoundMessageListenerMap.FindOrAdd(PendingListener.MessageId).AddUnique(Endpoint);
        }
        else
        {
            ASYNC_MESSAGE_WARNING("Endpoint became invalid while binding listener for message: %s",
                                 *PendingListener.MessageId.ToString());
        }
    }

    // Clear processed bindings
    PendingBoundListenerQueue.Empty();
}
```## Fase 6: Integración con World Subsystem

### Paso 10: Implementar World Subsystem

**¿Por qué necesitamos un World Subsystem?**
El World Subsystem gestiona la instancia del sistema de mensajes por mundo. En Unreal Engine, cada mundo (nivel) puede tener su propio sistema de mensajes independiente.

**DIFERENCIAS CRÍTICAS con el sistema original:**
- **ESPMode::NotThreadSafe**: Los shared pointers son más eficientes
- **Inicialización simple**: No necesitamos configurar sistemas de threading
- **Limpieza directa**: Sin shutdown complejo de threads

**10.1 Crear Public/AsyncMessageWorldSubsystem.h**

**¿Por qué este diseño es más simple?**
Sin threading, la gestión del ciclo de vida del subsystem es mucho más directa. No necesitamos coordinar el shutdown entre múltiples threads.
```cpp
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AsyncMessageSystemBase.h"
#include "AsyncMessageWorldSubsystem.generated.h"

UCLASS()
class ASYNCMESSAGESYSTEM_API UAsyncMessageWorldSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // USubsystem interface
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // UWorldSubsystem interface
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

    // Get the shared message system for a world
    static TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> GetSharedMessageSystem(UWorld* World);

    // Get the message system for this world
    TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> GetMessageSystem() const { return MessageSystem; }

protected:
    // CAMBIO CRÍTICO: ESPMode::NotThreadSafe
    // La instancia del sistema de mensajes para este mundo
    // Original: TSharedPtr<FAsyncMessageSystemBase, ESPMode::ThreadSafe> MessageSystem;
    // Nuevo: ESPMode::NotThreadSafe para mejor rendimiento
    TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> MessageSystem;
};
````

**10.2 Crear Private/AsyncMessageWorldSubsystem.cpp**

**¿Por qué esta implementación es más limpia?**
Sin threading, la inicialización y limpieza del subsystem es directa. No necesitamos esperar a que terminen threads o limpiar recursos de sincronización.

```cpp
#include "AsyncMessageWorldSubsystem.h"
#include "AsyncMessageSystemLogs.h"
#include "Engine/World.h"

void UAsyncMessageWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // INICIALIZACIÓN SIMPLE: Solo crear la instancia
    // DIFERENCIA: En el sistema original aquí se inicializarían:
    // - Thread pools para el mundo
    // - Sistemas de sincronización
    // - Configuración de task priorities
    MessageSystem = MakeShared<FAsyncMessageSystemBase, ESPMode::NotThreadSafe>();

    ASYNC_MESSAGE_LOG("AsyncMessageWorldSubsystem initialized for world: %s (Single-Thread)",
                     *GetNameSafe(GetWorld()));
}

void UAsyncMessageWorldSubsystem::Deinitialize()
{
    if (MessageSystem.IsValid())
    {
        ASYNC_MESSAGE_LOG("AsyncMessageWorldSubsystem deinitializing for world: %s",
                         *GetNameSafe(GetWorld()));

        // LIMPIEZA SIMPLE: Solo resetear el shared pointer
        // DIFERENCIA: En el sistema original aquí se haría:
        // - Shutdown de threads activos
        // - Flush de colas thread-safe
        // - Cleanup de recursos de sincronización
        // - Esperar a que terminen tareas pendientes
        MessageSystem.Reset();
    }

    Super::Deinitialize();
}

bool UAsyncMessageWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    // Create subsystem for all world types except editor preview worlds
    if (UWorld* World = Cast<UWorld>(Outer))
    {
        return World->WorldType != EWorldType::EditorPreview;
    }

    return false;
}

TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> UAsyncMessageWorldSubsystem::GetSharedMessageSystem(UWorld* World)
{
    if (!World)
    {
        ASYNC_MESSAGE_ERROR("Cannot get message system for null world");
        return nullptr;
    }

    UAsyncMessageWorldSubsystem* Subsystem = World->GetSubsystem<UAsyncMessageWorldSubsystem>();
    if (!Subsystem)
    {
        ASYNC_MESSAGE_ERROR("AsyncMessageWorldSubsystem not found for world: %s", *GetNameSafe(World));
        return nullptr;
    }

    return Subsystem->GetMessageSystem();
}
```

### Paso 11: Implementar Gameplay Message System

**¿Para qué sirve el Gameplay Message System?**
Esta es una especialización del sistema base que permite control más granular sobre cuándo se procesan los mensajes según los tick groups de Unreal Engine.

**DIFERENCIAS con el sistema original:**

- **Sin async tasks**: No spawneamos tareas asíncronas
- **Solo tick groups**: Procesamiento basado únicamente en tick groups
- **Control simple**: Registro/desregistro directo sin sincronización

**11.1 Crear Public/AsyncGameplayMessageSystem.h**

**¿Por qué heredar de FAsyncMessageSystemBase?**
Reutilizamos toda la funcionalidad base y solo agregamos control de tick groups. Es un patrón de especialización limpio.

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AsyncMessageSystemBase.h"
#include "Engine/Engine.h"

// Sistema de mensajes específico para gameplay con procesamiento por tick
// HERENCIA SIMPLE: Extiende funcionalidad base sin complejidad de threading
class ASYNCMESSAGESYSTEM_API FAsyncGameplayMessageSystem : public FAsyncMessageSystemBase
{
public:
    FAsyncGameplayMessageSystem();
    virtual ~FAsyncGameplayMessageSystem();

    // OVERRIDE: Procesamiento de tick para manejar tick groups
    // DIFERENCIA: En el sistema original también manejaba async tasks
    virtual void ExecuteTick(float DeltaTime, ETickingGroup TickGroup) override;

    // Registrarse para procesamiento en tick group específico
    // SIMPLIFICADO: No necesita sincronización para modificar el set
    void RegisterForTickGroup(ETickingGroup TickGroup);
    void UnregisterFromTickGroup(ETickingGroup TickGroup);

private:
    // Tick groups registrados
    // CONTAINER SIMPLE: TSet normal, no thread-safe
    TSet<ETickingGroup> RegisteredTickGroups;

    // ELIMINADO del sistema original:
    // - TMap<ETickingGroup, TSharedPtr<TaskSystem>> TaskSystems;
    // - FCriticalSection TickGroupCS;
    // - Async task spawning logic
};
```

**11.2 Crear Private/AsyncGameplayMessageSystem.cpp**

**¿Por qué esta implementación es tan directa?**
Sin threading, el control de tick groups es simple: solo verificamos si estamos registrados y llamamos al padre. No hay coordinación entre threads.

```cpp
#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageSystemLogs.h"

FAsyncGameplayMessageSystem::FAsyncGameplayMessageSystem()
{
    ASYNC_MESSAGE_LOG("AsyncGameplayMessageSystem created");

    // Register for default tick group
    RegisterForTickGroup(TG_DuringPhysics);
}

FAsyncGameplayMessageSystem::~FAsyncGameplayMessageSystem()
{
    ASYNC_MESSAGE_LOG("AsyncGameplayMessageSystem destroyed");
}

void FAsyncGameplayMessageSystem::ExecuteTick(float DeltaTime, ETickingGroup TickGroup)
{
    // FILTRO SIMPLE: Solo procesar si estamos registrados para este tick group
    // DIFERENCIA: En el sistema original aquí había:
    // - Locks para acceder a RegisteredTickGroups
    // - Spawning de async tasks por tick group
    // - Coordinación entre múltiples threads
    if (!RegisteredTickGroups.Contains(TickGroup))
    {
        return;
    }

    // DELEGACIÓN DIRECTA: Llamar implementación del padre
    // Todo el procesamiento real ocurre en FAsyncMessageSystemBase::ExecuteTick
    Super::ExecuteTick(DeltaTime, TickGroup);
}

void FAsyncGameplayMessageSystem::RegisterForTickGroup(ETickingGroup TickGroup)
{
    // OPERACIÓN SIMPLE: Agregar al set sin locks
    // DIFERENCIA: En el sistema original requería:
    // - FScopeLock Lock(&TickGroupCS);
    // - Inicialización de task systems por tick group
    // - Configuración de thread priorities
    RegisteredTickGroups.Add(TickGroup);
    ASYNC_MESSAGE_LOG("Registered for tick group: %d", (int32)TickGroup);
}

void FAsyncGameplayMessageSystem::UnregisterFromTickGroup(ETickingGroup TickGroup)
{
    // REMOCIÓN DIRECTA: Sin necesidad de sincronización
    // DIFERENCIA: En el sistema original requería:
    // - Locks para acceso thread-safe
    // - Shutdown de task systems asociados
    // - Cleanup de threads activos
    if (RegisteredTickGroups.Remove(TickGroup) > 0)
    {
        ASYNC_MESSAGE_LOG("Unregistered from tick group: %d", (int32)TickGroup);
    }
}
```

## Fase 7: Integración con Blueprints

### Paso 12: Implementar Blueprint Library

**¿Por qué es importante la integración con Blueprints?**
Los Blueprints permiten que diseñadores y programadores visuales usen el sistema de mensajes sin escribir C++. Es crucial mantener esta funcionalidad.

**DIFERENCIAS con el sistema original:**

- **Sin consideraciones de threading**: Las funciones Blueprint no necesitan ser thread-safe
- **Validación simplificada**: Sin verificar contextos de thread
- **Acceso directo**: Sin locks para acceder al sistema de mensajes

**12.1 Crear Private/AsyncMessageBlueprintLibrary.h**

**¿Por qué estas funciones son más simples?**
Sin threading, no necesitamos verificar desde qué thread se llaman las funciones Blueprint ni usar locks para acceder a datos compartidos.

````cpp
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AsyncMessageId.h"
#include "AsyncMessageHandle.h"
#include "AsyncMessageBindingEndpoint.h"
#include "StructUtils/InstancedStruct.h"
#include "AsyncMessageBlueprintLibrary.generated.h"

UCLASS()
class ASYNCMESSAGESYSTEM_API UAsyncMessageSystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // Queue a message for broadcast
    UFUNCTION(BlueprintCallable, Category = "Async Message System",
              meta = (WorldContext = "WorldContextObject"))
    static bool QueueAsyncMessageForBroadcast(
        UObject* WorldContextObject,
        const FAsyncMessageId& MessageId,
        const FInstancedStruct& Payload,
        TScriptInterface<IAsyncMessageBindingEndpointInterface> DesiredEndpoint = nullptr);

    // Create a message ID from string
    UFUNCTION(BlueprintPure, Category = "Async Message System")
    static FAsyncMessageId MakeAsyncMessageId(const FString& MessageName);

    // Check if message ID is valid
    UFUNCTION(BlueprintPure, Category = "Async Message System")
    static bool IsValidAsyncMessageId(const FAsyncMessageId& MessageId);

    // Check if message handle is valid
    UFUNCTION(BlueprintPure, Category = "Async Message System")
    static bool IsValidAsyncMessageHandle(const FAsyncMessageHandle& Handle);

    // Get debug information
    UFUNCTION(BlueprintCallable, Category = "Async Message System|Debug",
              meta = (WorldContext = "WorldContextObject"))
    static int32 GetQueuedMessageCount(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Async Message System|Debug",
              meta = (WorldContext = "WorldContextObject"))
    static int32 GetStoredMessageCount(UObject* WorldContextObject);
};
```**1
2.2 Create Private/AsyncMessageBlueprintLibrary.cpp**
```cpp
#include "AsyncMessageBlueprintLibrary.h"
#include "AsyncMessageWorldSubsystem.h"
#include "AsyncMessageSystemLogs.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

bool UAsyncMessageSystemBlueprintLibrary::QueueAsyncMessageForBroadcast(
    UObject* WorldContextObject,
    const FAsyncMessageId& MessageId,
    const FInstancedStruct& Payload,
    TScriptInterface<IAsyncMessageBindingEndpointInterface> DesiredEndpoint)
{
    check(GEngine);

    // OBTENER MUNDO: Validación estándar de contexto
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        ASYNC_MESSAGE_ERROR("Failed to queue message '%s' for broadcasting: Unable to find a world.",
                           *MessageId.ToString());
        return false;
    }

    // OBTENER SISTEMA: Acceso directo sin locks
    // DIFERENCIA: En el sistema original podría requerir locks si el sistema
    // se accediera desde múltiples threads
    TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> Sys = UAsyncMessageWorldSubsystem::GetSharedMessageSystem(World);
    if (!Sys.IsValid())
    {
        ASYNC_MESSAGE_ERROR("Failed to queue message '%s' for broadcasting: Unable to find a message system for world '%s'.",
                           *MessageId.ToString(), *GetNameSafe(World));
        return false;
    }

    // CONVERSIÓN DE ENDPOINT: Simple conversión de interface a weak pointer
    TWeakPtr<FAsyncMessageBindingEndpoint> WeakEndpoint = DesiredEndpoint ? DesiredEndpoint->GetEndpoint() : nullptr;

    // ENCOLADO DIRECTO: Sin verificaciones de thread context
    return Sys->QueueMessageForBroadcast(MessageId, Payload, WeakEndpoint);
}

FAsyncMessageId UAsyncMessageSystemBlueprintLibrary::MakeAsyncMessageId(const FString& MessageName)
{
    return FAsyncMessageId(MessageName);
}

bool UAsyncMessageSystemBlueprintLibrary::IsValidAsyncMessageId(const FAsyncMessageId& MessageId)
{
    return MessageId.IsValid();
}

bool UAsyncMessageSystemBlueprintLibrary::IsValidAsyncMessageHandle(const FAsyncMessageHandle& Handle)
{
    return Handle.IsValid();
}

int32 UAsyncMessageSystemBlueprintLibrary::GetQueuedMessageCount(UObject* WorldContextObject)
{
    check(GEngine);
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return 0;
    }

    TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> Sys = UAsyncMessageWorldSubsystem::GetSharedMessageSystem(World);
    if (!Sys.IsValid())
    {
        return 0;
    }

    return Sys->GetQueuedMessageCount();
}

int32 UAsyncMessageSystemBlueprintLibrary::GetStoredMessageCount(UObject* WorldContextObject)
{
    check(GEngine);
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        return 0;
    }

    TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> Sys = UAsyncMessageWorldSubsystem::GetSharedMessageSystem(World);
    if (!Sys.IsValid())
    {
        return 0;
    }

    return Sys->GetMessageStore().GetMessageCount();
}
````

### Paso 13: Implementar Message Binding Component

**¿Qué es el Message Binding Component?**
Es un componente que se puede agregar a cualquier Actor para que pueda recibir mensajes. Simplifica el uso del sistema desde Blueprints.

**DIFERENCIAS CRÍTICAS con el sistema original:**

- **Endpoint simple**: ESPMode::NotThreadSafe para mejor rendimiento
- **Lifecycle directo**: BeginPlay/EndPlay sin coordinación de threads
- **Cleanup automático**: Sin necesidad de sincronizar shutdown entre threads

**13.1 Crear Public/AsyncMessageBindingComponent.h**

**¿Por qué este componente es más simple?**
Sin threading, el ciclo de vida del componente es directo: se crea en BeginPlay, funciona durante el juego, y se limpia en EndPlay. No hay coordinación compleja.

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AsyncMessageBindingEndpoint.h"
#include "AsyncMessageId.h"
#include "AsyncMessageHandle.h"
#include "AsyncMessageBindingOptions.h"
#include "AsyncMessageBindingComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup=(AsyncMessageSystem),
       meta=(BlueprintSpawnableComponent))
class ASYNCMESSAGESYSTEM_API UAsyncMessageBindingComponent : public UActorComponent,
                                                            public IAsyncMessageBindingEndpointInterface
{
    GENERATED_BODY()

public:
    UAsyncMessageBindingComponent();

protected:
    // UActorComponent interface
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    // IAsyncMessageBindingEndpointInterface interface
    virtual TWeakPtr<FAsyncMessageBindingEndpoint> GetEndpoint() override;

    // Bind a listener to a message
    UFUNCTION(BlueprintCallable, Category = "Async Message System")
    FAsyncMessageHandle BindMessageListener(const FAsyncMessageId& MessageId,
                                           const FAsyncMessageBindingOptions& Options = FAsyncMessageBindingOptions());

    // Unbind a listener
    UFUNCTION(BlueprintCallable, Category = "Async Message System")
    void UnbindMessageListener(const FAsyncMessageHandle& Handle);

    // Blueprint event for receiving messages
    UFUNCTION(BlueprintImplementableEvent, Category = "Async Message System")
    void OnMessageReceived(const FAsyncMessageId& MessageId, const FInstancedStruct& Payload);

protected:
    // Internal callback for message reception
    void HandleMessageReceived(const FAsyncMessage& Message);

private:
    // The endpoint for this component
    TSharedPtr<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe> Endpoint;

    // Active message handles for cleanup
    TArray<FAsyncMessageHandle> ActiveHandles;
};
```

**13.2 Create Private/AsyncMessageBindingComponent.cpp**

````cpp
#include "AsyncMessageBindingComponent.h"
#include "AsyncMessageWorldSubsystem.h"
#include "AsyncMessageSystemLogs.h"
#include "Engine/World.h"

UAsyncMessageBindingComponent::UAsyncMessageBindingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Create the endpoint
    Endpoint = MakeShared<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe>();
}

void UAsyncMessageBindingComponent::BeginPlay()
{
    Super::BeginPlay();

    ASYNC_MESSAGE_LOG("AsyncMessageBindingComponent BeginPlay for actor: %s",
                     *GetNameSafe(GetOwner()));
}

void UAsyncMessageBindingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Clean up all active handles
    for (const FAsyncMessageHandle& Handle : ActiveHandles)
    {
        UnbindMessageListener(Handle);
    }
    ActiveHandles.Empty();

    // Reset endpoint
    if (Endpoint.IsValid())
    {
        Endpoint.Reset();
    }

    ASYNC_MESSAGE_LOG("AsyncMessageBindingComponent EndPlay for actor: %s",
                     *GetNameSafe(GetOwner()));

    Super::EndPlay(EndPlayReason);
}

TWeakPtr<FAsyncMessageBindingEndpoint> UAsyncMessageBindingComponent::GetEndpoint()
{
    return Endpoint;
}

FAsyncMessageHandle UAsyncMessageBindingComponent::BindMessageListener(const FAsyncMessageId& MessageId,
                                                                      const FAsyncMessageBindingOptions& Options)
{
    if (!MessageId.IsValid())
    {
        ASYNC_MESSAGE_ERROR("Cannot bind listener for invalid message ID in component: %s",
                           *GetNameSafe(this));
        return FAsyncMessageHandle();
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        ASYNC_MESSAGE_ERROR("Cannot bind listener - no world found for component: %s",
                           *GetNameSafe(this));
        return FAsyncMessageHandle();
    }

    TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> MessageSystem =
        UAsyncMessageWorldSubsystem::GetSharedMessageSystem(World);

    if (!MessageSystem.IsValid())
    {
        ASYNC_MESSAGE_ERROR("Cannot bind listener - no message system found for component: %s",
                           *GetNameSafe(this));
        return FAsyncMessageHandle();
    }

    // Create callback that will trigger Blueprint event
    FMessageCallbackFunc Callback;
    Callback.BindUObject(this, &UAsyncMessageBindingComponent::HandleMessageReceived);

    FAsyncMessageHandle Handle = MessageSystem->BindListener(MessageId, Options, Callback, Endpoint);

    if (Handle.IsValid())
    {
        ActiveHandles.Add(Handle);
        ASYNC_MESSAGE_LOG("Bound message listener %s for message: %s in component: %s",
                         *Handle.ToString(), *MessageId.ToString(), *GetNameSafe(this));
    }

    return Handle;
}

void UAsyncMessageBindingComponent::UnbindMessageListener(const FAsyncMessageHandle& Handle)
{
    if (!Handle.IsValid())
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> MessageSystem =
        UAsyncMessageWorldSubsystem::GetSharedMessageSystem(World);

    if (MessageSystem.IsValid())
    {
        MessageSystem->UnbindListener(Handle);
    }

    // Remove from active handles
    ActiveHandles.Remove(Handle);

    ASYNC_MESSAGE_LOG("Unbound message listener %s in component: %s",
                     *Handle.ToString(), *GetNameSafe(this));
}

void UAsyncMessageBindingComponent::HandleMessageReceived(const FAsyncMessage& Message)
{
    // Trigger Blueprint event
    OnMessageReceived(Message.GetMessageId(), Message.GetPayload());
}
```## Fase 8: Configuraciones de Desarrollador y Acciones Asíncronas

### Paso 14: Implementar Developer Settings

**¿Para qué sirven las Developer Settings?**
Permiten configurar el comportamiento del plugin desde el editor de Unreal Engine. Son especialmente útiles para debugging y optimización de rendimiento.

**DIFERENCIAS con el sistema original:**
- **Configuraciones simplificadas**: Sin opciones relacionadas con threading
- **Límites de rendimiento**: Enfocados en single-thread performance
- **Sin configuración de threads**: No hay opciones de thread pools o priorities

**14.1 Crear Private/AsyncMessageDeveloperSettings.h**

**¿Por qué estas configuraciones son diferentes?**
En single-thread, nos enfocamos en límites de procesamiento por tick en lugar de configuraciones de threading.
```cpp
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AsyncMessageDeveloperSettings.generated.h"

UCLASS(config=Game, defaultconfig, meta=(DisplayName="Async Message System"))
class ASYNCMESSAGESYSTEM_API UAsyncMessageDeveloperSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UAsyncMessageDeveloperSettings();

    // Enable debug logging
    UPROPERTY(config, EditAnywhere, Category = "Debug",
              meta = (DisplayName = "Enable Debug Logging"))
    bool bEnableDebugLogging;

    // CONFIGURACIÓN CLAVE: Máximo de mensajes por tick
    // DIFERENCIA: En el sistema original esto sería "Max Messages Per Thread"
    // En single-thread, limitamos cuántos mensajes procesamos por frame
    UPROPERTY(config, EditAnywhere, Category = "Performance",
              meta = (DisplayName = "Max Messages Per Tick", ClampMin = "1", ClampMax = "1000"))
    int32 MaxMessagesPerTick;

    // Maximum stored messages per message ID
    UPROPERTY(config, EditAnywhere, Category = "Performance",
              meta = (DisplayName = "Max Stored Messages Per ID", ClampMin = "1", ClampMax = "10000"))
    int32 MaxStoredMessagesPerID;

    // Enable message storage
    UPROPERTY(config, EditAnywhere, Category = "Storage",
              meta = (DisplayName = "Enable Message Storage"))
    bool bEnableMessageStorage;

    // Auto-cleanup stored messages after time (seconds)
    UPROPERTY(config, EditAnywhere, Category = "Storage",
              meta = (DisplayName = "Auto Cleanup Time (seconds)", ClampMin = "0.0"))
    float AutoCleanupTime;

    // UDeveloperSettings interface
    virtual FName GetCategoryName() const override;

#if WITH_EDITOR
    virtual FText GetSectionText() const override;
    virtual FText GetSectionDescription() const override;
#endif
};
````

**14.2 Crear Private/AsyncMessageDeveloperSettings.cpp**

**¿Por qué estos valores por defecto?**
Los valores están optimizados para single-thread performance. MaxMessagesPerTick de 100 es un buen balance entre responsividad y rendimiento.

```cpp
#include "AsyncMessageDeveloperSettings.h"

UAsyncMessageDeveloperSettings::UAsyncMessageDeveloperSettings()
    : bEnableDebugLogging(false)
    , MaxMessagesPerTick(100)        // OPTIMIZADO: Para single-thread performance
    , MaxStoredMessagesPerID(1000)   // LÍMITE: Previene uso excesivo de memoria
    , bEnableMessageStorage(true)    // HABILITADO: Storage es útil para debugging
    , AutoCleanupTime(300.0f)        // 5 minutos: Balance entre utilidad y memoria
{
    // DIFERENCIA: En el sistema original también habría:
    // - MaxThreads, ThreadPoolSize, TaskPriorities, etc.
    // - Configuraciones de sincronización y locks
}

FName UAsyncMessageDeveloperSettings::GetCategoryName() const
{
    return FName(TEXT("Plugins"));
}

#if WITH_EDITOR
FText UAsyncMessageDeveloperSettings::GetSectionText() const
{
    return NSLOCTEXT("AsyncMessageSystem", "SettingsDisplayName", "Async Message System");
}

FText UAsyncMessageDeveloperSettings::GetSectionDescription() const
{
    return NSLOCTEXT("AsyncMessageSystem", "SettingsDescription",
                    "Configure the Async Message System plugin settings");
}
#endif
```

### Step 15: Implement Async Action for Blueprint Message Listening

**15.1 Create Private/AsyncAction_ListenForMessage.h**

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "AsyncMessageId.h"
#include "AsyncMessageHandle.h"
#include "AsyncMessageBindingOptions.h"
#include "AsyncMessageBindingEndpoint.h"
#include "StructUtils/InstancedStruct.h"
#include "AsyncAction_ListenForMessage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncMessageReceivedDelegate,
                                             FAsyncMessageId, MessageId,
                                             FInstancedStruct, Payload);

UCLASS()
class ASYNCMESSAGESYSTEM_API UAsyncAction_ListenForMessage : public UCancellableAsyncAction,
                                                            public IAsyncMessageBindingEndpointInterface
{
    GENERATED_BODY()

public:
    // Blueprint event delegates
    UPROPERTY(BlueprintAssignable)
    FAsyncMessageReceivedDelegate OnMessageReceived;

    // Create and start listening for a message
    UFUNCTION(BlueprintCallable, Category = "Async Message System",
              meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
    static UAsyncAction_ListenForMessage* ListenForMessage(
        UObject* WorldContextObject,
        const FAsyncMessageId& MessageId,
        const FAsyncMessageBindingOptions& Options = FAsyncMessageBindingOptions());

    // UCancellableAsyncAction interface
    virtual void Cancel() override;

    // IAsyncMessageBindingEndpointInterface interface
    virtual TWeakPtr<FAsyncMessageBindingEndpoint> GetEndpoint() override;

protected:
    // UBlueprintAsyncActionBase interface
    virtual void Activate() override;

private:
    // Internal callback for message reception
    void HandleMessageReceived(const FAsyncMessage& Message);

    // Parameters
    TWeakObjectPtr<UObject> WorldContextObject;
    FAsyncMessageId MessageId;
    FAsyncMessageBindingOptions Options;

    // Runtime data
    TSharedPtr<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe> Endpoint;
    FAsyncMessageHandle ListenerHandle;
    bool bIsActive;
};
```

**15.2 Create Private/AsyncAction_ListenForMessage.cpp**

````cpp
#include "AsyncAction_ListenForMessage.h"
#include "AsyncMessageWorldSubsystem.h"
#include "AsyncMessageSystemLogs.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

UAsyncAction_ListenForMessage* UAsyncAction_ListenForMessage::ListenForMessage(
    UObject* WorldContextObject,
    const FAsyncMessageId& MessageId,
    const FAsyncMessageBindingOptions& Options)
{
    UAsyncAction_ListenForMessage* Action = NewObject<UAsyncAction_ListenForMessage>();
    Action->WorldContextObject = WorldContextObject;
    Action->MessageId = MessageId;
    Action->Options = Options;
    Action->bIsActive = false;

    // Create endpoint
    Action->Endpoint = MakeShared<FAsyncMessageBindingEndpoint, ESPMode::NotThreadSafe>();

    return Action;
}

void UAsyncAction_ListenForMessage::Activate()
{
    // VALIDACIÓN BÁSICA: Verificar que el MessageId sea válido
    if (!MessageId.IsValid())
    {
        ASYNC_MESSAGE_ERROR("Cannot listen for invalid message ID in AsyncAction");
        Cancel();
        return;
    }

    // OBTENER CONTEXTO: Verificar que el contexto del mundo siga válido
    UObject* WorldContext = WorldContextObject.Get();
    if (!WorldContext)
    {
        ASYNC_MESSAGE_ERROR("World context became invalid in AsyncAction");
        Cancel();
        return;
    }

    // OBTENER MUNDO: Estándar de Unreal Engine
    check(GEngine);
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        ASYNC_MESSAGE_ERROR("Cannot find world for AsyncAction");
        Cancel();
        return;
    }

    // OBTENER SISTEMA: Acceso directo sin locks
    // DIFERENCIA: En el sistema original podría requerir verificación de thread context
    TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> MessageSystem =
        UAsyncMessageWorldSubsystem::GetSharedMessageSystem(World);

    if (!MessageSystem.IsValid())
    {
        ASYNC_MESSAGE_ERROR("Cannot find message system for AsyncAction");
        Cancel();
        return;
    }

    // CREAR CALLBACK: Binding directo a método de esta clase
    FMessageCallbackFunc Callback;
    Callback.BindUObject(this, &UAsyncAction_ListenForMessage::HandleMessageReceived);

    // VINCULAR LISTENER: Operación simple sin consideraciones de threading
    ListenerHandle = MessageSystem->BindListener(MessageId, Options, Callback, Endpoint);

    if (ListenerHandle.IsValid())
    {
        bIsActive = true;
        ASYNC_MESSAGE_LOG("AsyncAction started listening for message: %s", *MessageId.ToString());
    }
    else
    {
        ASYNC_MESSAGE_ERROR("Failed to bind listener in AsyncAction for message: %s", *MessageId.ToString());
        Cancel();
    }
}

void UAsyncAction_ListenForMessage::Cancel()
{
    if (bIsActive && ListenerHandle.IsValid())
    {
        UObject* WorldContext = WorldContextObject.Get();
        if (WorldContext)
        {
            check(GEngine);
            UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
            if (World)
            {
                TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> MessageSystem =
                    UAsyncMessageWorldSubsystem::GetSharedMessageSystem(World);

                if (MessageSystem.IsValid())
                {
                    MessageSystem->UnbindListener(ListenerHandle);
                }
            }
        }

        bIsActive = false;
        ListenerHandle = FAsyncMessageHandle();

        ASYNC_MESSAGE_LOG("AsyncAction cancelled listening for message: %s", *MessageId.ToString());
    }

    // Reset endpoint
    if (Endpoint.IsValid())
    {
        Endpoint.Reset();
    }

    Super::Cancel();
}

TWeakPtr<FAsyncMessageBindingEndpoint> UAsyncAction_ListenForMessage::GetEndpoint()
{
    return Endpoint;
}

void UAsyncAction_ListenForMessage::HandleMessageReceived(const FAsyncMessage& Message)
{
    if (bIsActive)
    {
        // Broadcast to Blueprint
        OnMessageReceived.Broadcast(Message.GetMessageId(), Message.GetPayload());
    }
}
```## Phase 9:
Testing and Validation

### Step 16: Create Basic Unit Tests

**16.1 Create Private/Tests/AsyncMessageSystemTests.cpp**
```cpp
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "AsyncMessageId.h"
#include "AsyncMessageHandle.h"
#include "AsyncMessage.h"
#include "AsyncMessageSystemBase.h"
#include "StructUtils/InstancedStruct.h"

// Test message ID functionality
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAsyncMessageIdTest,
    "AsyncMessageSystem.Core.MessageId",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAsyncMessageIdTest::RunTest(const FString& Parameters)
{
    // Test valid message ID
    FAsyncMessageId ValidId("TestMessage");
    TestTrue("Valid message ID should be valid", ValidId.IsValid());
    TestEqual("Message ID string conversion", ValidId.ToString(), FString("TestMessage"));

    // Test invalid message ID
    FAsyncMessageId InvalidId;
    TestFalse("Default message ID should be invalid", InvalidId.IsValid());

    // Test comparison
    FAsyncMessageId SameId("TestMessage");
    TestTrue("Same message IDs should be equal", ValidId == SameId);

    return true;
}

// Test message handle functionality
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAsyncMessageHandleTest,
    "AsyncMessageSystem.Core.MessageHandle",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAsyncMessageHandleTest::RunTest(const FString& Parameters)
{
    // Test handle generation
    FAsyncMessageHandle Handle1 = FAsyncMessageHandle::GenerateNewHandle();
    FAsyncMessageHandle Handle2 = FAsyncMessageHandle::GenerateNewHandle();

    TestTrue("Generated handle should be valid", Handle1.IsValid());
    TestTrue("Second generated handle should be valid", Handle2.IsValid());
    TestTrue("Generated handles should be different", Handle1 != Handle2);

    // Test invalid handle
    FAsyncMessageHandle InvalidHandle;
    TestFalse("Default handle should be invalid", InvalidHandle.IsValid());

    return true;
}

// Test basic message system functionality
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAsyncMessageSystemBasicTest,
    "AsyncMessageSystem.Core.BasicFunctionality",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAsyncMessageSystemBasicTest::RunTest(const FString& Parameters)
{
    // CREAR SISTEMA: Instancia simple del sistema de mensajes
    // DIFERENCIA: En el sistema original también habría que configurar thread pools
    TSharedPtr<FAsyncMessageSystemBase, ESPMode::NotThreadSafe> MessageSystem =
        MakeShared<FAsyncMessageSystemBase, ESPMode::NotThreadSafe>();

    TestTrue("Message system should be valid", MessageSystem.IsValid());

    // PROBAR ENCOLADO: Verificar que los mensajes se encolan correctamente
    FAsyncMessageId TestMessageId("TestMessage");
    FInstancedStruct TestPayload;

    bool bQueued = MessageSystem->QueueMessageForBroadcast(TestMessageId, TestPayload);
    TestTrue("Message should be queued successfully", bQueued);
    TestEqual("Should have 1 queued message", MessageSystem->GetQueuedMessageCount(), 1);

    // PROBAR PROCESAMIENTO: Verificar que ExecuteTick procesa los mensajes
    // VENTAJA SINGLE-THREAD: Comportamiento completamente predecible
    MessageSystem->ExecuteTick(0.016f, TG_DuringPhysics);
    TestEqual("Messages should be processed after tick", MessageSystem->GetQueuedMessageCount(), 0);

    return true;
}
````

### Paso 17: Integración Final y Limpieza

**¿Por qué necesitamos este paso final?**
Es crucial verificar que toda la migración funcione correctamente y que no hayamos introducido bugs al eliminar el threading.

**17.1 Completar implementación de AsyncMessageHandle.cpp**

**¿Por qué esta implementación es simple?**
En el sistema original, GetBindingEndpoint requeriría acceso thread-safe a un registry global. En single-thread, simplificamos devolviendo nullptr ya que los endpoints se gestionan directamente por el sistema.

```cpp
// Actualizar el método GetBindingEndpoint en AsyncMessageHandle.cpp
TSharedPtr<FAsyncMessageBindingEndpoint> FAsyncMessageHandle::GetBindingEndpoint() const
{
    // IMPLEMENTACIÓN SIMPLIFICADA: En el sistema original esto requeriría:
    // - Locks para acceder a un registry global thread-safe
    // - Búsqueda en estructuras de datos protegidas por mutexes
    // - Verificación de validez desde múltiples threads

    // En single-thread: Los endpoints se gestionan directamente por el sistema
    // No necesitamos un registry global complejo
    return nullptr;
}
```

**17.2 Lista de validación final**

## Lista de Validación

### Pruebas de Compilación

**¿Por qué estas pruebas son críticas?**
Verifican que hayamos eliminado correctamente todas las dependencias de threading sin romper la funcionalidad básica.

- [ ] **Plugin compila sin errores**: Verificar que no hay referencias a APIs de threading eliminadas
- [ ] **Sin dependencias de threading**: Confirmar que no quedan imports de "Tasks" u otros módulos de threading
- [ ] **Headers correctos**: Todos los archivos tienen los includes necesarios para single-thread
- [ ] **Funciones Blueprint expuestas**: Las macros UFUNCTION funcionan correctamente

### Pruebas de Funcionalidad

**¿Por qué estas pruebas son esenciales?**
Garantizan que la funcionalidad core se preserve completamente a pesar de los cambios arquitectónicos.

- [ ] **Broadcasting funciona**: Los mensajes se envían correctamente
- [ ] **Listening funciona**: Los listeners reciben mensajes apropiadamente
- [ ] **Integración Blueprint**: Todas las funciones Blueprint operan correctamente
- [ ] **Ciclo de vida de componentes**: BeginPlay/EndPlay funcionan sin problemas
- [ ] **World subsystem se inicializa**: El subsystem se crea y destruye correctamente

### Performance Tests

- [ ] Single-threaded performance is acceptable
- [ ] No memory leaks in message processing
- [ ] Tick processing stays within budget
- [ ] Message storage doesn't grow unbounded

## ¡Migración Completada!

Has creado exitosamente una versión single-threaded del plugin AsyncMessageSystem.

## Resumen de Cambios Críticos

### Componentes de Threading Eliminados:

**1. Colas Thread-Safe → Arrays Simples**

- **Antes**: `TMpscQueue<FQueuedMessage>` (Multi-Producer Single-Consumer)
- **Después**: `TArray<FQueuedMessage>`
- **¿Por qué?**: En single-thread no necesitamos sincronización entre productores

**2. Variables Atómicas → Enteros Regulares**

- **Antes**: `std::atomic<uint32> NextHandleId`
- **Después**: `uint32 NextHandleId`
- **¿Por qué?**: Solo un thread genera IDs, no hay condiciones de carrera

**3. Critical Sections Eliminados**

- **Antes**: `FCriticalSection MessageListenerMapCS`, `FScopeLock`
- **Después**: Acceso directo sin locks
- **¿Por qué?**: Sin acceso concurrente, no necesitamos protección

**4. Shared Pointers Simplificados**

- **Antes**: `ESPMode::ThreadSafe`
- **Después**: `ESPMode::NotThreadSafe`
- **¿Por qué?**: Mejor rendimiento sin overhead de thread-safety

**5. Procesamiento Asíncrono → Tick-Based**

- **Antes**: `UE::Tasks`, `ENamedThreads`, async task spawning
- **Después**: Procesamiento durante `ExecuteTick()`
- **¿Por qué?**: Flujo de ejecución más simple y predecible

### Funcionalidad Preservada:

- ✅ Broadcasting y listening de mensajes
- ✅ Integración con Blueprints
- ✅ Endpoints basados en componentes
- ✅ Integración con world subsystem
- ✅ Configuraciones de desarrollador y debugging
- ✅ Almacenamiento y recuperación de mensajes

### Beneficios Logrados:

**🎯 Arquitectura Simplificada**

- Código más fácil de leer y entender
- Menos abstracciones y capas de complejidad
- Flujo de ejecución lineal y predecible

**🐛 Debugging Más Fácil**

- Sin condiciones de carrera
- Stack traces más claros
- Comportamiento determinístico

**⚡ Mejor Rendimiento**

- Sin overhead de locks y atomics
- Mejor localidad de cache
- Menos cambios de contexto

**📚 Mejor Experiencia de Aprendizaje**

- Conceptos más fáciles de entender
- Código más directo y menos "mágico"
- Ideal para entender sistemas de mensajería

**🔧 Mantenimiento Simplificado**

- Menos puntos de falla
- Testing más predecible
- Refactoring más seguro

## Cuándo Usar Esta Versión

**✅ Ideal para:**

- Proyectos de aprendizaje
- Juegos single-player
- Sistemas con volumen moderado de mensajes
- Cuando la simplicidad es prioritaria
- Debugging y prototipado

**⚠️ Considerar alternativas para:**

- Sistemas con miles de mensajes por frame
- Cuando necesitas procesamiento en background threads
- Aplicaciones que requieren máximo rendimiento

## Próximos Pasos

El plugin está listo para usar y personalizar. Puedes:

1. **Extender funcionalidad**: Agregar nuevos tipos de mensajes
2. **Optimizar**: Implementar pooling de objetos si es necesario
3. **Personalizar**: Agregar filtros o transformaciones de mensajes
4. **Integrar**: Conectar con otros sistemas de tu juego

¡Has aprendido los conceptos fundamentales de sistemas de mensajería sin la complejidad del multi-threading!

```

```
