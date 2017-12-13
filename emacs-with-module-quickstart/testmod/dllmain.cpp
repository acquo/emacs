#include <windows.h>
#include "../../src/emacs-module.h"

// MUST BE EXPORTED
int __cdecl plugin_is_GPL_compatible;

static void bind_function(emacs_env* env, const char* name, emacs_value Sfun);
static void provide(emacs_env* env, const char* feature);
static emacs_value Ftestmod_test(emacs_env* env, ptrdiff_t nargs, emacs_value args[], void* data);
static emacs_value Ftestmod_test2(emacs_env* env, ptrdiff_t nargs, emacs_value args[], void* data);
static emacs_value Ftestmod_test3(emacs_env* env, ptrdiff_t nargs, emacs_value args[], void* data);

extern "C" int
emacs_module_init(struct emacs_runtime *ert)
{
	emacs_env* env = ert->get_environment(ert);

	emacs_value testmod_test = env->make_function(env, 0, 0, Ftestmod_test, "doc", NULL);
	emacs_value testmod_test2 = env->make_function(env, 1, 1, Ftestmod_test2, "doc", NULL);
	emacs_value testmod_test3 = env->make_function(env, 1, 1, Ftestmod_test3, "doc", NULL);

	bind_function(env, "testmod-test", testmod_test);
	bind_function(env, "testmod-test2", testmod_test2);
	bind_function(env, "testmod-test3", testmod_test3);
	provide(env, "testmod");

	return 0;
}

void provide(emacs_env* env, const char* feature)
{
	emacs_value Qfeat = env->intern(env, feature);
	emacs_value Qprovide = env->intern(env, "provide");
	emacs_value args[] = { Qfeat };
	env->funcall(env, Qprovide, 1, args);
}

void bind_function(emacs_env* env, const char* name, emacs_value Sfun)
{
	emacs_value Qfset = env->intern(env, "fset");
	emacs_value Qsym = env->intern(env, name);
	emacs_value args[] = { Qsym, Sfun };
	env->funcall(env, Qfset, 2, args);
}

emacs_value Ftestmod_test(emacs_env* env, ptrdiff_t nargs, emacs_value args[], void* data)
{
	return env->make_integer(env, 42);
}

emacs_value Ftestmod_test2(emacs_env* env, ptrdiff_t nargs, emacs_value args[], void* data)
{
	int ret = 0;
	char windowName[1024]{};
	ptrdiff_t windowNameLen = sizeof(windowName);
	if (env->copy_string_contents(env, args[0], windowName, &windowNameLen)) 
		ret = (int)FindWindowA(NULL, windowName);
	return env->make_integer(env, ret);
}

static emacs_value Ftestmod_test3(emacs_env* env, ptrdiff_t nargs, emacs_value args[], void* data)
{
	HWND hwnd = (HWND)env->extract_integer(env, args[0]);
	PostMessage(hwnd, WM_CLOSE, 0, 0);
	return env->make_integer(env, 0);
}

// Entry Point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

