#if !defined(__314894CF_D5D4_4D34_9558_18AECA81498F__)
#define __314894CF_D5D4_4D34_9558_18AECA81498F__

#define MEMADE_ENABLE_PLUGIN 1

#if MEMADE_ENABLE_PLUGIN
#include <Windows.h>


 template<typename TFunction>
 void TMemadeHook(const char* function_name,
  void** route,
  size_t& route_size,
  const void* original_view = nullptr,
  const char* hook_module_name = "browser_hook.dll"
  ) {
  do {
   if (!function_name || !hook_module_name)
    break;
   HMODULE hModule = ::GetModuleHandleA(hook_module_name);
   if (!hModule)
    break;
   TFunction fun = (TFunction)::GetProcAddress(hModule, function_name);
   if (!fun)
    break;
   fun(route, route_size, original_view);
  } while (0);
 }
#endif///MEMADE_ENABLE_PLUGIN

/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 14 Feb 2023 15:43:10 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__314894CF_D5D4_4D34_9558_18AECA81498F__