/* Copyright (C) 2023 Gleb Bezborodov - All Rights Reserved
* You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/TinyTls
 */

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTinyTls, Log, All);

/* Module definition for tls stream manager. Is intended to startup/shutdown tls stuff*/
class TINYTLS_API FTinyTlsModule final : public FDefaultModuleImpl
{
public:
	// Begin IModuleInterface overrides
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface overrides
};
