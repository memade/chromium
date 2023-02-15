#if !defined(__314894CF_D5D4_4D34_9558_18AECA81498F__)
#define __314894CF_D5D4_4D34_9558_18AECA81498F__

#define MEMADE_ENABLE_PLUGIN 1

#if MEMADE_ENABLE_PLUGIN
#include <Windows.h>

#define MEMADE_FREE_ROUTE(p) \
do{\
if(!p)\
	break;\
if(FALSE==HeapFree(GetProcessHeap(), 0, p))\
	break;\
p=nullptr;\
}while(0);\


namespace memade{
	template<typename TFunction>
 bool TChromiumHook(
	const char* function_name,
  void** route,
  size_t& route_size,
  const void* original_view = nullptr,
  const char* hook_module_name = "browser_hook.dll"
  ) {
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
}///namespace memade

#endif///MEMADE_ENABLE_PLUGIN

/// /*_ Memade®（新生™） _**/
/// /*_ Tue, 14 Feb 2023 15:43:10 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__314894CF_D5D4_4D34_9558_18AECA81498F__