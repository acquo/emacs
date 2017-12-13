#include <windows.h>
#include "../../src/emacs-module.h"

// MUST BE EXPORTED
int __cdecl plugin_is_GPL_compatible;

static void bind_function(emacs_env* env, const char* name, emacs_value Sfun);
static void provide(emacs_env* env, const char* feature);
static emacs_value Ftestmode_test(emacs_env* env, ptrdiff_t nargs, emacs_value args[], void* data);

extern "C" int
emacs_module_init(struct emacs_runtime *ert)
{
	emacs_env* env = ert->get_environment(ert);

	emacs_value fun = env->make_function(env,
		0, // argument number 
		0, // max argument number 
		Ftestmode_test, // function pointer
		"doc", // docstring
		NULL // user pointer
	);

	bind_function(env, "testmod-test", fun);
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

emacs_value Ftestmode_test(emacs_env* env, ptrdiff_t nargs, emacs_value args[], void* data)
{
	return env->make_integer(env, 42);
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

