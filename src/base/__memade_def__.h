#if !defined(__314894CF_D5D4_4D34_9558_18AECA81498F__)
#define __314894CF_D5D4_4D34_9558_18AECA81498F__

#include <Windows.h>
#define MEMADE_MAIN_SWITCH 1
#define MEMADE_BROWSER_PROCESS_MAIN_HOOK 1
#define MEMADE_BROWSER_PROCESS_CHEILD_HOOK 1
#define MEMADE_BROWSER_PROCESS_MAIN_HOOK_MODULE_NAME L"MAIN.DLL"
#define MEMADE_BROWSER_PROCESS_CHILD_HOOK_MODULE_NAME L"CHROME_ELF.DLL"
#define MEMADE_PLUGIN_FUNCTION_INIT "api_object_init"
#define MEMADE_PLUGIN_FUNCTION_UNINIT "api_object_uninit"

#define MEMADE_VNAME(value) (#value)

using tf_memade_plugin_init = void(__stdcall*)(void**, unsigned long&);
using tf_memade_plugin_uninit = void(__stdcall*)(void);

#define MEMADE_LOADLIBRARY(hModule,dllfname)\
do {\
hModule = ::GetModuleHandleW(MEMADE_BROWSER_PROCESS_MAIN_HOOK_MODULE_NAME);\
if (!hModule){hModule = LoadLibraryW(\
dllfname!=nullptr?\
(LPCWSTR)dllfname:\
(LPCWSTR)MEMADE_BROWSER_PROCESS_MAIN_HOOK_MODULE_NAME); }\
if (!hModule){	break;}\
auto plugin_init =(tf_memade_plugin_init)::GetProcAddress(hModule, MEMADE_PLUGIN_FUNCTION_INIT); \
void* route = nullptr;\
unsigned long route_size = 0;\
if (plugin_init){plugin_init(&route, route_size);}\
} while (0); \

#define MEMADE_UNLOADLIBRARY(hModule)\
do {\
if (!hModule){break;}\
auto plugin_uninit =(tf_memade_plugin_uninit)::GetProcAddress(hModule, MEMADE_PLUGIN_FUNCTION_UNINIT); \
if (plugin_uninit){plugin_uninit();}\
::FreeLibrary(hModule);\
hModule = nullptr;\
} while (0);\

#if 0
#define MEMADE_ENABLE_PLUGIN 1
#if MEMADE_ENABLE_PLUGIN
#include <Windows.h>
#define MEMADE_FREE_ROUTE(p)\
do {\
if (!p)\
break;\
if (FALSE == HeapFree(GetProcessHeap(), 0, p)) \
break;\
p = nullptr;\
} while (0);\
#define MEMADE_PLUGIN_INI(hModule)\
do {\
hModule = ::GetModuleHandleW(memade::MEMADE_PLUGIN_PE_FILENAME_MAIN_W);\
if (!hModule)\
hModule = LoadLibraryW(memade::MEMADE_PLUGIN_PE_FILENAME_MAIN_W);\
if (!hModule)\
break;\
memade::tf_plugin_init fn_init =\
(memade::tf_plugin_init)::GetProcAddress(hModule, "api_object_init"); \
if (fn_init)\
fn_init(nullptr, 0);\
} while (0);\
#define MEMADE_PLUGIN_UNINIT(hModule)\
do {\
if (!hModule)\
break;\
memade::tf_plugin_uninit fn_uninit =(memade::tf_plugin_uninit)::GetProcAddress(\
hModule, "api_object_uninit"); \
if (fn_uninit)\
fn_uninit();\
::FreeLibrary(hModule);\
hModule = nullptr;\
} while (0);\
namespace memade {
using tf_plugin_init = void* (__stdcall*)(const void*, unsigned long);
using tf_plugin_uninit = void(__stdcall*)(void);
static const char MEMADE_PLUGIN_PE_FILENAME_A[] = "chrome_elf.dll";
static const wchar_t MEMADE_PLUGIN_PE_FILENAME_W[] = L"chrome_elf.dll";
static const char MEMADE_PLUGIN_PE_FILENAME_MAIN_A[] = "memade_main.dll";
static const wchar_t MEMADE_PLUGIN_PE_FILENAME_MAIN_W[] = L"memade_main.dll";
template <typename TFunction>
bool TChromiumHook(
	const char* function_name,
	void** route,
	size_t& route_size,
	const void* original_view = nullptr,
	const char* hook_module_name = MEMADE_PLUGIN_PE_FILENAME_A) {
	bool result = false;
	do {
		if (!function_name || !hook_module_name)
			break;
		HMODULE hModule = ::GetModuleHandleA(hook_module_name);
		if (!hModule)
			break;
		TFunction fun = (TFunction)::GetProcAddress(hModule, function_name);
		if (!fun)
			break;
		result = fun(route, route_size, original_view);
	} while (0);
	return result;
}
}  // namespace memade
extern HMODULE g_hMemadePluginModule;
#endif  /// MEMADE_ENABLE_PLUGIN

#endif


#if MEMADE_BROWSER_PROCESS_CHEILD_HOOK


#endif///MEMADE_BROWSER_PROCESS_CHEILD_HOOK


#if MEMADE_MAIN_SWITCH && MEMADE_BROWSER_PROCESS_MAIN_HOOK
extern HMODULE __hMemadeBrowserMain;
#endif///#if MEMADE_MAIN_SWITCH && MEMADE_BROWSER_PROCESS_MAIN_HOOK
/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 14 Feb 2023 15:43:10 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif  ///__314894CF_D5D4_4D34_9558_18AECA81498F__