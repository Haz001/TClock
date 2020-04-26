// This file contains helper functions that are automatically created from
// templates.

#include "nuitka/prelude.h"

extern PyObject *callPythonFunction( PyObject *func, PyObject **args, int count );


PyObject *CALL_FUNCTION_WITH_ARGS1( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 1 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 1; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 1 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 1 * sizeof(PyObject *) );
            memcpy( python_pars + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 1 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 1 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 1; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 1 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 1 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 1 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 1 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (1 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 1 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (1 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 1 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 1, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 1 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            1
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 1 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS2( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 2; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 2 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 2; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 2 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 2 * sizeof(PyObject *) );
            memcpy( python_pars + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 2 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 2 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 2; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 2 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 2 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 2 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 2 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (2 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 2 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (2 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 2 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 2, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 2 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            2
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 2 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS3( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 3; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 3 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 3; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 3 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 3 * sizeof(PyObject *) );
            memcpy( python_pars + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 3 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 3 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 3; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 3 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 3 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 3 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 3 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (3 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 3 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (3 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 3 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 3, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 3 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            3
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 3 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS4( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 4; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 4 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 4; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 4 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 4 * sizeof(PyObject *) );
            memcpy( python_pars + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 4 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 4 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 4; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 4 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 4 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 4 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 4 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (4 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 4 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (4 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 4 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 4, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 4 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            4
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 4 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS5( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 5; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 5 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 5; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 5 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 5 * sizeof(PyObject *) );
            memcpy( python_pars + 5, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 5 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 5 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 5; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 5 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 5 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 5, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 5 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 5 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (5 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 5 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (5 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 5 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 5, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 5 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            5
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 5 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS6( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 6; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 6 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 6; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 6 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 6 * sizeof(PyObject *) );
            memcpy( python_pars + 6, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 6 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 6 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 6; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 6 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 6 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 6, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 6 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 6 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (6 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 6 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (6 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 6 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 6, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 6 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            6
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 6 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_METHOD_WITH_ARGS1( PyObject *source, PyObject *attr_name, PyObject **args )
{
    CHECK_OBJECT( source );
    CHECK_OBJECT( attr_name );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    PyTypeObject *type = Py_TYPE( source );

    if ( type->tp_getattro == PyObject_GenericGetAttr )
    {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely( type->tp_dict == NULL ))
        {
            if (unlikely( PyType_Ready( type ) < 0 ))
            {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup( type, attr_name );
        descrgetfunc func = NULL;

        if ( descr != NULL )
        {
            Py_INCREF( descr );

#if PYTHON_VERSION < 300
            if ( PyType_HasFeature( Py_TYPE( descr ), Py_TPFLAGS_HAVE_CLASS ) )
            {
#endif
                func = Py_TYPE( descr )->tp_descr_get;

                if ( func != NULL && PyDescr_IsData( descr ) )
                {
                    PyObject *called_object = func( descr, source, (PyObject *)type );
                    Py_DECREF( descr );

                    PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                        called_object,
                        args
                    );
                    Py_DECREF( called_object );
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if ( dict != NULL )
        {
            CHECK_OBJECT( dict );

            Py_INCREF( dict );

            PyObject *called_object = PyDict_GetItem( dict, attr_name );

            if ( called_object != NULL )
            {
                Py_INCREF( called_object );
                Py_XDECREF( descr );
                Py_DECREF( dict );

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF( called_object );
                return result;
            }

            Py_DECREF( dict );
        }

        if ( func != NULL )
        {
            if ( func == Nuitka_Function_Type.tp_descr_get )
            {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    1
                );

                Py_DECREF( descr );

                return result;
            }
            else
            {
                PyObject *called_object = func( descr, source, (PyObject *)type );
                CHECK_OBJECT( called_object );

                Py_DECREF( descr );

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF( called_object );

                return result;
            }
        }

        if ( descr != NULL )
        {
            CHECK_OBJECT( descr );

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                descr,
                args
            );
            Py_DECREF( descr );

            return result;
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if ( type == &PyInstance_Type )
    {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert( attr_name != const_str_plain___dict__ );
        assert( attr_name != const_str_plain___class__ );

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            return CALL_FUNCTION_WITH_ARGS1( called_object, args );
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            descrgetfunc descr_get = Py_TYPE( called_object )->tp_descr_get;

            if ( descr_get == Nuitka_Function_Type.tp_descr_get )
            {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    1
                );
            }
            else if ( descr_get != NULL )
            {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely( method == NULL ))
                {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS1( method, args );
                Py_DECREF( method );
                return result;
            }
            else
            {
                return CALL_FUNCTION_WITH_ARGS1( called_object, args );
            }

        }
        else if (unlikely( source_instance->in_class->cl_getattr == NULL ))
        {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        }
        else
        {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely( called_object == NULL ))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                called_object,
                args
            );
            Py_DECREF( called_object );
            return result;
        }
    }
#endif
    else if ( type->tp_getattro != NULL )
    {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else if ( type->tp_getattr != NULL )
    {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            (char *)Nuitka_String_AsString_Unchecked( attr_name )
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else
    {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked( attr_name )
        );

        return NULL;
    }
}

PyObject *CALL_METHOD_WITH_ARGS2( PyObject *source, PyObject *attr_name, PyObject **args )
{
    CHECK_OBJECT( source );
    CHECK_OBJECT( attr_name );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 2; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    PyTypeObject *type = Py_TYPE( source );

    if ( type->tp_getattro == PyObject_GenericGetAttr )
    {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely( type->tp_dict == NULL ))
        {
            if (unlikely( PyType_Ready( type ) < 0 ))
            {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup( type, attr_name );
        descrgetfunc func = NULL;

        if ( descr != NULL )
        {
            Py_INCREF( descr );

#if PYTHON_VERSION < 300
            if ( PyType_HasFeature( Py_TYPE( descr ), Py_TPFLAGS_HAVE_CLASS ) )
            {
#endif
                func = Py_TYPE( descr )->tp_descr_get;

                if ( func != NULL && PyDescr_IsData( descr ) )
                {
                    PyObject *called_object = func( descr, source, (PyObject *)type );
                    Py_DECREF( descr );

                    PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                        called_object,
                        args
                    );
                    Py_DECREF( called_object );
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if ( dict != NULL )
        {
            CHECK_OBJECT( dict );

            Py_INCREF( dict );

            PyObject *called_object = PyDict_GetItem( dict, attr_name );

            if ( called_object != NULL )
            {
                Py_INCREF( called_object );
                Py_XDECREF( descr );
                Py_DECREF( dict );

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF( called_object );
                return result;
            }

            Py_DECREF( dict );
        }

        if ( func != NULL )
        {
            if ( func == Nuitka_Function_Type.tp_descr_get )
            {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    2
                );

                Py_DECREF( descr );

                return result;
            }
            else
            {
                PyObject *called_object = func( descr, source, (PyObject *)type );
                CHECK_OBJECT( called_object );

                Py_DECREF( descr );

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF( called_object );

                return result;
            }
        }

        if ( descr != NULL )
        {
            CHECK_OBJECT( descr );

            PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                descr,
                args
            );
            Py_DECREF( descr );

            return result;
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if ( type == &PyInstance_Type )
    {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert( attr_name != const_str_plain___dict__ );
        assert( attr_name != const_str_plain___class__ );

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            return CALL_FUNCTION_WITH_ARGS2( called_object, args );
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            descrgetfunc descr_get = Py_TYPE( called_object )->tp_descr_get;

            if ( descr_get == Nuitka_Function_Type.tp_descr_get )
            {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    2
                );
            }
            else if ( descr_get != NULL )
            {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely( method == NULL ))
                {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS2( method, args );
                Py_DECREF( method );
                return result;
            }
            else
            {
                return CALL_FUNCTION_WITH_ARGS2( called_object, args );
            }

        }
        else if (unlikely( source_instance->in_class->cl_getattr == NULL ))
        {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        }
        else
        {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely( called_object == NULL ))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                called_object,
                args
            );
            Py_DECREF( called_object );
            return result;
        }
    }
#endif
    else if ( type->tp_getattro != NULL )
    {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else if ( type->tp_getattr != NULL )
    {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            (char *)Nuitka_String_AsString_Unchecked( attr_name )
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else
    {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked( attr_name )
        );

        return NULL;
    }
}
/* Code to register embedded modules for meta path based loading if any. */

#include "nuitka/unfreezing.h"

/* Table for lookup to find compiled or bytecode modules included in this
 * binary or module, or put along this binary as extension modules. We do
 * our own loading for each of these.
 */
MOD_INIT_DECL(__main__);
static struct Nuitka_MetaPathBasedLoaderEntry meta_path_loader_entries[] =
{
    { "__main__", MOD_INIT_NAME( __main__ ), 0, 0,  },
    { "_bsddb", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_cn", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_hk", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_iso2022", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_jp", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_kr", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_tw", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_csv", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ctypes", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_curses", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_curses_panel", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_elementtree", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_hashlib", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_hotshot", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_json", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_lsprof", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_multibytecodec", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_multiprocessing", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_sqlite3", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ssl", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "bz2", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "dbm", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "mmap", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "os", NULL, 741, 25575, NUITKA_BYTECODE_FLAG },
    { "parser", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "readline", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "resource", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "termios", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "BaseHTTPServer", NULL, 26316, 21674, NUITKA_BYTECODE_FLAG },
    { "Bastion", NULL, 47990, 6622, NUITKA_BYTECODE_FLAG },
    { "CGIHTTPServer", NULL, 54612, 10984, NUITKA_BYTECODE_FLAG },
    { "Canvas", NULL, 65596, 15396, NUITKA_BYTECODE_FLAG },
    { "ConfigParser", NULL, 80992, 25087, NUITKA_BYTECODE_FLAG },
    { "Cookie", NULL, 106079, 22574, NUITKA_BYTECODE_FLAG },
    { "Dialog", NULL, 128653, 1902, NUITKA_BYTECODE_FLAG },
    { "DocXMLRPCServer", NULL, 130555, 10149, NUITKA_BYTECODE_FLAG },
    { "FileDialog", NULL, 140704, 9666, NUITKA_BYTECODE_FLAG },
    { "FixTk", NULL, 150370, 2078, NUITKA_BYTECODE_FLAG },
    { "HTMLParser", NULL, 152448, 13655, NUITKA_BYTECODE_FLAG },
    { "MimeWriter", NULL, 166103, 7338, NUITKA_BYTECODE_FLAG },
    { "Queue", NULL, 173441, 9360, NUITKA_BYTECODE_FLAG },
    { "ScrolledText", NULL, 182801, 2646, NUITKA_BYTECODE_FLAG },
    { "SimpleDialog", NULL, 185447, 4309, NUITKA_BYTECODE_FLAG },
    { "SimpleHTTPServer", NULL, 189756, 7980, NUITKA_BYTECODE_FLAG },
    { "SimpleXMLRPCServer", NULL, 197736, 22783, NUITKA_BYTECODE_FLAG },
    { "SocketServer", NULL, 220519, 23948, NUITKA_BYTECODE_FLAG },
    { "StringIO", NULL, 244467, 11434, NUITKA_BYTECODE_FLAG },
    { "Tix", NULL, 255901, 95416, NUITKA_BYTECODE_FLAG },
    { "Tkconstants", NULL, 351317, 2236, NUITKA_BYTECODE_FLAG },
    { "Tkdnd", NULL, 353553, 12765, NUITKA_BYTECODE_FLAG },
    { "Tkinter", NULL, 366318, 199207, NUITKA_BYTECODE_FLAG },
    { "UserDict", NULL, 565525, 9613, NUITKA_BYTECODE_FLAG },
    { "UserList", NULL, 575138, 6501, NUITKA_BYTECODE_FLAG },
    { "UserString", NULL, 581639, 14720, NUITKA_BYTECODE_FLAG },
    { "_LWPCookieJar", NULL, 596359, 5414, NUITKA_BYTECODE_FLAG },
    { "_MozillaCookieJar", NULL, 601773, 4445, NUITKA_BYTECODE_FLAG },
    { "__future__", NULL, 606218, 4203, NUITKA_BYTECODE_FLAG },
    { "_abcoll", NULL, 610421, 25466, NUITKA_BYTECODE_FLAG },
    { "_osx_support", NULL, 635887, 11712, NUITKA_BYTECODE_FLAG },
    { "_pyio", NULL, 647599, 64343, NUITKA_BYTECODE_FLAG },
    { "_strptime", NULL, 711942, 15124, NUITKA_BYTECODE_FLAG },
    { "_sysconfigdata", NULL, 727066, 271, NUITKA_BYTECODE_FLAG },
    { "_sysconfigdata_nd", NULL, 727337, 20828, NUITKA_BYTECODE_FLAG },
    { "_threading_local", NULL, 748165, 6347, NUITKA_BYTECODE_FLAG },
    { "_weakrefset", NULL, 754512, 9574, NUITKA_BYTECODE_FLAG },
    { "abc", NULL, 764086, 6113, NUITKA_BYTECODE_FLAG },
    { "aifc", NULL, 770199, 30305, NUITKA_BYTECODE_FLAG },
    { "anydbm", NULL, 800504, 2786, NUITKA_BYTECODE_FLAG },
    { "argparse", NULL, 803290, 64049, NUITKA_BYTECODE_FLAG },
    { "ast", NULL, 867339, 12882, NUITKA_BYTECODE_FLAG },
    { "asynchat", NULL, 880221, 8742, NUITKA_BYTECODE_FLAG },
    { "asyncore", NULL, 888963, 18767, NUITKA_BYTECODE_FLAG },
    { "atexit", NULL, 907730, 2183, NUITKA_BYTECODE_FLAG },
    { "audiodev", NULL, 909913, 8407, NUITKA_BYTECODE_FLAG },
    { "bdb", NULL, 918320, 18971, NUITKA_BYTECODE_FLAG },
    { "binhex", NULL, 937291, 15350, NUITKA_BYTECODE_FLAG },
    { "bisect", NULL, 952641, 3053, NUITKA_BYTECODE_FLAG },
    { "bsddb", NULL, 955694, 12368, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "bsddb.db", NULL, 968062, 582, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbobj", NULL, 968644, 18601, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbrecio", NULL, 987245, 5269, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbshelve", NULL, 992514, 12910, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbtables", NULL, 1005424, 24402, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbutils", NULL, 1029826, 1615, NUITKA_BYTECODE_FLAG },
    { "cProfile", NULL, 1031441, 6263, NUITKA_BYTECODE_FLAG },
    { "calendar", NULL, 1037704, 27747, NUITKA_BYTECODE_FLAG },
    { "cgi", NULL, 1065451, 32417, NUITKA_BYTECODE_FLAG },
    { "cgitb", NULL, 1097868, 12094, NUITKA_BYTECODE_FLAG },
    { "chunk", NULL, 1109962, 5572, NUITKA_BYTECODE_FLAG },
    { "cmd", NULL, 1115534, 13989, NUITKA_BYTECODE_FLAG },
    { "code", NULL, 1129523, 10294, NUITKA_BYTECODE_FLAG },
    { "codeop", NULL, 1139817, 6569, NUITKA_BYTECODE_FLAG },
    { "collections", NULL, 1146386, 26051, NUITKA_BYTECODE_FLAG },
    { "colorsys", NULL, 1172437, 3967, NUITKA_BYTECODE_FLAG },
    { "commands", NULL, 1176404, 2449, NUITKA_BYTECODE_FLAG },
    { "compileall", NULL, 1178853, 6997, NUITKA_BYTECODE_FLAG },
    { "compiler", NULL, 1185850, 1287, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "compiler.ast", NULL, 1187137, 71339, NUITKA_BYTECODE_FLAG },
    { "compiler.consts", NULL, 1258476, 727, NUITKA_BYTECODE_FLAG },
    { "compiler.future", NULL, 1259203, 2928, NUITKA_BYTECODE_FLAG },
    { "compiler.misc", NULL, 1262131, 3687, NUITKA_BYTECODE_FLAG },
    { "compiler.pyassem", NULL, 1265818, 25783, NUITKA_BYTECODE_FLAG },
    { "compiler.pycodegen", NULL, 1291601, 56161, NUITKA_BYTECODE_FLAG },
    { "compiler.symbols", NULL, 1347762, 17557, NUITKA_BYTECODE_FLAG },
    { "compiler.syntax", NULL, 1365319, 1862, NUITKA_BYTECODE_FLAG },
    { "compiler.transformer", NULL, 1367181, 47387, NUITKA_BYTECODE_FLAG },
    { "compiler.visitor", NULL, 1414568, 4159, NUITKA_BYTECODE_FLAG },
    { "contextlib", NULL, 1418727, 4422, NUITKA_BYTECODE_FLAG },
    { "cookielib", NULL, 1423149, 54366, NUITKA_BYTECODE_FLAG },
    { "copy", NULL, 1477515, 12110, NUITKA_BYTECODE_FLAG },
    { "csv", NULL, 1489625, 13441, NUITKA_BYTECODE_FLAG },
    { "ctypes", NULL, 1503066, 20224, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes._endian", NULL, 1523290, 2287, NUITKA_BYTECODE_FLAG },
    { "ctypes.util", NULL, 1525577, 8518, NUITKA_BYTECODE_FLAG },
    { "curses", NULL, 1534095, 1539, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "curses.ascii", NULL, 1535634, 5085, NUITKA_BYTECODE_FLAG },
    { "curses.has_key", NULL, 1540719, 5928, NUITKA_BYTECODE_FLAG },
    { "curses.panel", NULL, 1546647, 267, NUITKA_BYTECODE_FLAG },
    { "curses.textpad", NULL, 1546914, 7121, NUITKA_BYTECODE_FLAG },
    { "curses.wrapper", NULL, 1554035, 1206, NUITKA_BYTECODE_FLAG },
    { "dbhash", NULL, 1555241, 706, NUITKA_BYTECODE_FLAG },
    { "decimal", NULL, 1555947, 171637, NUITKA_BYTECODE_FLAG },
    { "difflib", NULL, 1727584, 61782, NUITKA_BYTECODE_FLAG },
    { "dircache", NULL, 1789366, 1560, NUITKA_BYTECODE_FLAG },
    { "dis", NULL, 1790926, 6204, NUITKA_BYTECODE_FLAG },
    { "distutils", NULL, 1797130, 405, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.archive_util", NULL, 1797535, 7383, NUITKA_BYTECODE_FLAG },
    { "distutils.bcppcompiler", NULL, 1804918, 7856, NUITKA_BYTECODE_FLAG },
    { "distutils.ccompiler", NULL, 1812774, 36746, NUITKA_BYTECODE_FLAG },
    { "distutils.cmd", NULL, 1849520, 16722, NUITKA_BYTECODE_FLAG },
    { "distutils.command", NULL, 1866242, 655, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.command.bdist", NULL, 1866897, 5149, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_dumb", NULL, 1872046, 4963, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_msi", NULL, 1877009, 23917, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_rpm", NULL, 1900926, 17580, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_wininst", NULL, 1918506, 10878, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build", NULL, 1929384, 5115, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_clib", NULL, 1934499, 6319, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_ext", NULL, 1940818, 19252, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_py", NULL, 1960070, 11443, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_scripts", NULL, 1971513, 4455, NUITKA_BYTECODE_FLAG },
    { "distutils.command.check", NULL, 1975968, 6192, NUITKA_BYTECODE_FLAG },
    { "distutils.command.clean", NULL, 1982160, 3051, NUITKA_BYTECODE_FLAG },
    { "distutils.command.config", NULL, 1985211, 12637, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install", NULL, 1997848, 17884, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_data", NULL, 2015732, 3096, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_egg_info", NULL, 2018828, 4345, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_headers", NULL, 2023173, 2233, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_lib", NULL, 2025406, 6649, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_scripts", NULL, 2032055, 2906, NUITKA_BYTECODE_FLAG },
    { "distutils.command.register", NULL, 2034961, 10174, NUITKA_BYTECODE_FLAG },
    { "distutils.command.sdist", NULL, 2045135, 16652, NUITKA_BYTECODE_FLAG },
    { "distutils.command.upload", NULL, 2061787, 6288, NUITKA_BYTECODE_FLAG },
    { "distutils.config", NULL, 2068075, 3543, NUITKA_BYTECODE_FLAG },
    { "distutils.core", NULL, 2071618, 7520, NUITKA_BYTECODE_FLAG },
    { "distutils.cygwinccompiler", NULL, 2079138, 9793, NUITKA_BYTECODE_FLAG },
    { "distutils.debug", NULL, 2088931, 244, NUITKA_BYTECODE_FLAG },
    { "distutils.dep_util", NULL, 2089175, 3164, NUITKA_BYTECODE_FLAG },
    { "distutils.dir_util", NULL, 2092339, 6764, NUITKA_BYTECODE_FLAG },
    { "distutils.dist", NULL, 2099103, 39034, NUITKA_BYTECODE_FLAG },
    { "distutils.emxccompiler", NULL, 2138137, 7441, NUITKA_BYTECODE_FLAG },
    { "distutils.errors", NULL, 2145578, 6237, NUITKA_BYTECODE_FLAG },
    { "distutils.extension", NULL, 2151815, 7396, NUITKA_BYTECODE_FLAG },
    { "distutils.fancy_getopt", NULL, 2159211, 11908, NUITKA_BYTECODE_FLAG },
    { "distutils.file_util", NULL, 2171119, 6732, NUITKA_BYTECODE_FLAG },
    { "distutils.filelist", NULL, 2177851, 10714, NUITKA_BYTECODE_FLAG },
    { "distutils.log", NULL, 2188565, 2754, NUITKA_BYTECODE_FLAG },
    { "distutils.msvc9compiler", NULL, 2191319, 21427, NUITKA_BYTECODE_FLAG },
    { "distutils.msvccompiler", NULL, 2212746, 17465, NUITKA_BYTECODE_FLAG },
    { "distutils.spawn", NULL, 2230211, 6405, NUITKA_BYTECODE_FLAG },
    { "distutils.sysconfig", NULL, 2236616, 15079, NUITKA_BYTECODE_FLAG },
    { "distutils.text_file", NULL, 2251695, 9229, NUITKA_BYTECODE_FLAG },
    { "distutils.unixccompiler", NULL, 2260924, 8087, NUITKA_BYTECODE_FLAG },
    { "distutils.util", NULL, 2269011, 14304, NUITKA_BYTECODE_FLAG },
    { "distutils.version", NULL, 2283315, 7170, NUITKA_BYTECODE_FLAG },
    { "distutils.versionpredicate", NULL, 2290485, 5520, NUITKA_BYTECODE_FLAG },
    { "doctest", NULL, 2296005, 83440, NUITKA_BYTECODE_FLAG },
    { "dumbdbm", NULL, 2379445, 6700, NUITKA_BYTECODE_FLAG },
    { "dummy_thread", NULL, 2386145, 5356, NUITKA_BYTECODE_FLAG },
    { "dummy_threading", NULL, 2391501, 1275, NUITKA_BYTECODE_FLAG },
    { "email", NULL, 2392776, 2852, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email._parseaddr", NULL, 2395628, 13831, NUITKA_BYTECODE_FLAG },
    { "email.base64mime", NULL, 2409459, 5305, NUITKA_BYTECODE_FLAG },
    { "email.charset", NULL, 2414764, 13499, NUITKA_BYTECODE_FLAG },
    { "email.encoders", NULL, 2428263, 2210, NUITKA_BYTECODE_FLAG },
    { "email.errors", NULL, 2430473, 3491, NUITKA_BYTECODE_FLAG },
    { "email.feedparser", NULL, 2433964, 11516, NUITKA_BYTECODE_FLAG },
    { "email.generator", NULL, 2445480, 10334, NUITKA_BYTECODE_FLAG },
    { "email.header", NULL, 2455814, 13622, NUITKA_BYTECODE_FLAG },
    { "email.iterators", NULL, 2469436, 2348, NUITKA_BYTECODE_FLAG },
    { "email.message", NULL, 2471784, 28576, NUITKA_BYTECODE_FLAG },
    { "email.mime", NULL, 2500360, 120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email.mime.application", NULL, 2500480, 1570, NUITKA_BYTECODE_FLAG },
    { "email.mime.audio", NULL, 2502050, 2893, NUITKA_BYTECODE_FLAG },
    { "email.mime.base", NULL, 2504943, 1102, NUITKA_BYTECODE_FLAG },
    { "email.mime.image", NULL, 2506045, 2035, NUITKA_BYTECODE_FLAG },
    { "email.mime.message", NULL, 2508080, 1434, NUITKA_BYTECODE_FLAG },
    { "email.mime.multipart", NULL, 2509514, 1655, NUITKA_BYTECODE_FLAG },
    { "email.mime.nonmultipart", NULL, 2511169, 874, NUITKA_BYTECODE_FLAG },
    { "email.mime.text", NULL, 2512043, 1294, NUITKA_BYTECODE_FLAG },
    { "email.parser", NULL, 2513337, 3804, NUITKA_BYTECODE_FLAG },
    { "email.quoprimime", NULL, 2517141, 8816, NUITKA_BYTECODE_FLAG },
    { "email.utils", NULL, 2525957, 9284, NUITKA_BYTECODE_FLAG },
    { "encodings.mbcs", NULL, 2535241, 2019, NUITKA_BYTECODE_FLAG },
    { "filecmp", NULL, 2537260, 9574, NUITKA_BYTECODE_FLAG },
    { "fileinput", NULL, 2546834, 14432, NUITKA_BYTECODE_FLAG },
    { "fnmatch", NULL, 2561266, 3594, NUITKA_BYTECODE_FLAG },
    { "formatter", NULL, 2564860, 19016, NUITKA_BYTECODE_FLAG },
    { "fpformat", NULL, 2583876, 4679, NUITKA_BYTECODE_FLAG },
    { "fractions", NULL, 2588555, 19625, NUITKA_BYTECODE_FLAG },
    { "ftplib", NULL, 2608180, 34609, NUITKA_BYTECODE_FLAG },
    { "genericpath", NULL, 2642789, 3487, NUITKA_BYTECODE_FLAG },
    { "getopt", NULL, 2646276, 6626, NUITKA_BYTECODE_FLAG },
    { "getpass", NULL, 2652902, 4722, NUITKA_BYTECODE_FLAG },
    { "gettext", NULL, 2657624, 17993, NUITKA_BYTECODE_FLAG },
    { "glob", NULL, 2675617, 2919, NUITKA_BYTECODE_FLAG },
    { "gzip", NULL, 2678536, 15168, NUITKA_BYTECODE_FLAG },
    { "hashlib", NULL, 2693704, 6883, NUITKA_BYTECODE_FLAG },
    { "heapq", NULL, 2700587, 14520, NUITKA_BYTECODE_FLAG },
    { "hmac", NULL, 2715107, 4514, NUITKA_BYTECODE_FLAG },
    { "hotshot", NULL, 2719621, 3454, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "hotshot.log", NULL, 2723075, 5507, NUITKA_BYTECODE_FLAG },
    { "hotshot.stats", NULL, 2728582, 3374, NUITKA_BYTECODE_FLAG },
    { "hotshot.stones", NULL, 2731956, 1144, NUITKA_BYTECODE_FLAG },
    { "htmlentitydefs", NULL, 2733100, 6357, NUITKA_BYTECODE_FLAG },
    { "htmllib", NULL, 2739457, 20119, NUITKA_BYTECODE_FLAG },
    { "httplib", NULL, 2759576, 36779, NUITKA_BYTECODE_FLAG },
    { "ihooks", NULL, 2796355, 21226, NUITKA_BYTECODE_FLAG },
    { "imaplib", NULL, 2817581, 45161, NUITKA_BYTECODE_FLAG },
    { "imghdr", NULL, 2862742, 4798, NUITKA_BYTECODE_FLAG },
    { "importlib", NULL, 2867540, 1488, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "imputil", NULL, 2869028, 15539, NUITKA_BYTECODE_FLAG },
    { "inspect", NULL, 2884567, 40083, NUITKA_BYTECODE_FLAG },
    { "io", NULL, 2924650, 3571, NUITKA_BYTECODE_FLAG },
    { "json", NULL, 2928221, 13908, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "json.decoder", NULL, 2942129, 11928, NUITKA_BYTECODE_FLAG },
    { "json.encoder", NULL, 2954057, 13679, NUITKA_BYTECODE_FLAG },
    { "json.scanner", NULL, 2967736, 2215, NUITKA_BYTECODE_FLAG },
    { "json.tool", NULL, 2969951, 1282, NUITKA_BYTECODE_FLAG },
    { "keyword", NULL, 2971233, 2093, NUITKA_BYTECODE_FLAG },
    { "lib2to3", NULL, 2973326, 117, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.btm_matcher", NULL, 2973443, 5800, NUITKA_BYTECODE_FLAG },
    { "lib2to3.btm_utils", NULL, 2979243, 7529, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_base", NULL, 2986772, 7146, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_util", NULL, 2993918, 14607, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes", NULL, 3008525, 123, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.fixes.fix_apply", NULL, 3008648, 2070, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_asserts", NULL, 3010718, 1547, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_basestring", NULL, 3012265, 793, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_buffer", NULL, 3013058, 950, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_dict", NULL, 3014008, 3753, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_except", NULL, 3017761, 2993, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exec", NULL, 3020754, 1418, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_execfile", NULL, 3022172, 2059, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exitfunc", NULL, 3024231, 2739, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_filter", NULL, 3026970, 2256, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_funcattrs", NULL, 3029226, 1114, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_future", NULL, 3030340, 919, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_getcwdu", NULL, 3031259, 926, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_has_key", NULL, 3032185, 3184, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_idioms", NULL, 3035369, 4515, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_import", NULL, 3039884, 3233, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports", NULL, 3043117, 5352, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports2", NULL, 3048469, 622, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_input", NULL, 3049091, 1134, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_intern", NULL, 3050225, 1783, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_isinstance", NULL, 3052008, 1838, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools", NULL, 3053846, 1791, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools_imports", NULL, 3055637, 2016, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_long", NULL, 3057653, 841, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_map", NULL, 3058494, 3040, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_metaclass", NULL, 3061534, 6579, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_methodattrs", NULL, 3068113, 1138, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ne", NULL, 3069251, 985, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_next", NULL, 3070236, 3531, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_nonzero", NULL, 3073767, 1086, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_numliterals", NULL, 3074853, 1249, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_operator", NULL, 3076102, 5112, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_paren", NULL, 3081214, 1543, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_print", NULL, 3082757, 2727, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raise", NULL, 3085484, 2498, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raw_input", NULL, 3087982, 936, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reduce", NULL, 3088918, 1262, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_renames", NULL, 3090180, 2449, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_repr", NULL, 3092629, 1016, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_set_literal", NULL, 3093645, 1988, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_standarderror", NULL, 3095633, 853, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_sys_exc", NULL, 3096486, 1705, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_throw", NULL, 3098191, 1996, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_tuple_params", NULL, 3100187, 5430, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_types", NULL, 3105617, 2196, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_unicode", NULL, 3107813, 1716, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_urllib", NULL, 3109529, 7134, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ws_comma", NULL, 3116663, 1382, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xrange", NULL, 3118045, 3063, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xreadlines", NULL, 3121108, 1152, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_zip", NULL, 3122260, 1346, NUITKA_BYTECODE_FLAG },
    { "lib2to3.main", NULL, 3123606, 9803, NUITKA_BYTECODE_FLAG },
    { "lib2to3.patcomp", NULL, 3133409, 6569, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2", NULL, 3139978, 164, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.pgen2.conv", NULL, 3140142, 8169, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.driver", NULL, 3148311, 6434, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.grammar", NULL, 3154745, 7654, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.literals", NULL, 3162399, 2002, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.parse", NULL, 3164401, 7204, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.pgen", NULL, 3171605, 12166, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.token", NULL, 3183771, 2287, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.tokenize", NULL, 3186058, 16923, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pygram", NULL, 3202981, 1427, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pytree", NULL, 3204408, 30143, NUITKA_BYTECODE_FLAG },
    { "lib2to3.refactor", NULL, 3234551, 23866, NUITKA_BYTECODE_FLAG },
    { "linecache", NULL, 3258417, 3252, NUITKA_BYTECODE_FLAG },
    { "logging", NULL, 3261669, 57332, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "logging.config", NULL, 3319001, 25834, NUITKA_BYTECODE_FLAG },
    { "logging.handlers", NULL, 3344835, 39488, NUITKA_BYTECODE_FLAG },
    { "macpath", NULL, 3384323, 7635, NUITKA_BYTECODE_FLAG },
    { "macurl2path", NULL, 3391958, 2228, NUITKA_BYTECODE_FLAG },
    { "mailbox", NULL, 3394186, 76271, NUITKA_BYTECODE_FLAG },
    { "mailcap", NULL, 3470457, 7045, NUITKA_BYTECODE_FLAG },
    { "markupbase", NULL, 3477502, 9225, NUITKA_BYTECODE_FLAG },
    { "md5", NULL, 3486727, 368, NUITKA_BYTECODE_FLAG },
    { "mhlib", NULL, 3487095, 33613, NUITKA_BYTECODE_FLAG },
    { "mimetools", NULL, 3520708, 8153, NUITKA_BYTECODE_FLAG },
    { "mimetypes", NULL, 3528861, 18425, NUITKA_BYTECODE_FLAG },
    { "mimify", NULL, 3547286, 11963, NUITKA_BYTECODE_FLAG },
    { "modulefinder", NULL, 3559249, 19049, NUITKA_BYTECODE_FLAG },
    { "multifile", NULL, 3578298, 5382, NUITKA_BYTECODE_FLAG },
    { "multiprocessing", NULL, 3583680, 8405, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.connection", NULL, 3592085, 14310, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.dummy", NULL, 3606395, 5412, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.dummy.connection", NULL, 3611807, 2732, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.forking", NULL, 3614539, 14400, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.heap", NULL, 3628939, 6861, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.managers", NULL, 3635800, 38147, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.pool", NULL, 3673947, 22468, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.process", NULL, 3696415, 9544, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.queues", NULL, 3705959, 11421, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.reduction", NULL, 3717380, 5972, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.sharedctypes", NULL, 3723352, 8572, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.synchronize", NULL, 3731924, 10939, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.util", NULL, 3742863, 10081, NUITKA_BYTECODE_FLAG },
    { "mutex", NULL, 3752944, 2494, NUITKA_BYTECODE_FLAG },
    { "netrc", NULL, 3755438, 4688, NUITKA_BYTECODE_FLAG },
    { "new", NULL, 3760126, 852, NUITKA_BYTECODE_FLAG },
    { "nntplib", NULL, 3760978, 20948, NUITKA_BYTECODE_FLAG },
    { "ntpath", NULL, 3781926, 13077, NUITKA_BYTECODE_FLAG },
    { "nturl2path", NULL, 3795003, 1801, NUITKA_BYTECODE_FLAG },
    { "numbers", NULL, 3796804, 13880, NUITKA_BYTECODE_FLAG },
    { "opcode", NULL, 3810684, 6127, NUITKA_BYTECODE_FLAG },
    { "optparse", NULL, 3816811, 53606, NUITKA_BYTECODE_FLAG },
    { "os", NULL, 741, 25575, NUITKA_BYTECODE_FLAG },
    { "os2emxpath", NULL, 3870417, 4499, NUITKA_BYTECODE_FLAG },
    { "pdb", NULL, 3874916, 43433, NUITKA_BYTECODE_FLAG },
    { "pickle", NULL, 3918349, 38344, NUITKA_BYTECODE_FLAG },
    { "pickletools", NULL, 3956693, 56960, NUITKA_BYTECODE_FLAG },
    { "pipes", NULL, 4013653, 9268, NUITKA_BYTECODE_FLAG },
    { "pkgutil", NULL, 4022921, 18885, NUITKA_BYTECODE_FLAG },
    { "platform", NULL, 4041806, 38489, NUITKA_BYTECODE_FLAG },
    { "plistlib", NULL, 4080295, 19115, NUITKA_BYTECODE_FLAG },
    { "popen2", NULL, 4099410, 8985, NUITKA_BYTECODE_FLAG },
    { "poplib", NULL, 4108395, 13271, NUITKA_BYTECODE_FLAG },
    { "posixfile", NULL, 4121666, 7620, NUITKA_BYTECODE_FLAG },
    { "posixpath", NULL, 4129286, 11377, NUITKA_BYTECODE_FLAG },
    { "pprint", NULL, 4140663, 10148, NUITKA_BYTECODE_FLAG },
    { "profile", NULL, 4150811, 16370, NUITKA_BYTECODE_FLAG },
    { "pstats", NULL, 4167181, 24885, NUITKA_BYTECODE_FLAG },
    { "pty", NULL, 4192066, 4938, NUITKA_BYTECODE_FLAG },
    { "py_compile", NULL, 4197004, 6603, NUITKA_BYTECODE_FLAG },
    { "pyclbr", NULL, 4203607, 9617, NUITKA_BYTECODE_FLAG },
    { "pydoc", NULL, 4213224, 92384, NUITKA_BYTECODE_FLAG },
    { "pydoc_data", NULL, 4305608, 120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "pydoc_data.topics", NULL, 4305728, 420808, NUITKA_BYTECODE_FLAG },
    { "random", NULL, 4726536, 25610, NUITKA_BYTECODE_FLAG },
    { "repr", NULL, 4752146, 5343, NUITKA_BYTECODE_FLAG },
    { "rexec", NULL, 4757489, 23681, NUITKA_BYTECODE_FLAG },
    { "rfc822", NULL, 4781170, 31685, NUITKA_BYTECODE_FLAG },
    { "rlcompleter", NULL, 4812855, 6054, NUITKA_BYTECODE_FLAG },
    { "robotparser", NULL, 4818909, 7947, NUITKA_BYTECODE_FLAG },
    { "runpy", NULL, 4826856, 8755, NUITKA_BYTECODE_FLAG },
    { "sched", NULL, 4835611, 4968, NUITKA_BYTECODE_FLAG },
    { "sets", NULL, 4840579, 16775, NUITKA_BYTECODE_FLAG },
    { "sgmllib", NULL, 4857354, 15334, NUITKA_BYTECODE_FLAG },
    { "sha", NULL, 4872688, 411, NUITKA_BYTECODE_FLAG },
    { "shelve", NULL, 4873099, 10194, NUITKA_BYTECODE_FLAG },
    { "shlex", NULL, 4883293, 7524, NUITKA_BYTECODE_FLAG },
    { "shutil", NULL, 4890817, 18835, NUITKA_BYTECODE_FLAG },
    { "site", NULL, 4909652, 16582, NUITKA_BYTECODE_FLAG },
    { "sitecustomize", NULL, 4926234, 224, NUITKA_BYTECODE_FLAG },
    { "smtpd", NULL, 4926458, 15813, NUITKA_BYTECODE_FLAG },
    { "smtplib", NULL, 4942271, 30186, NUITKA_BYTECODE_FLAG },
    { "sndhdr", NULL, 4972457, 7319, NUITKA_BYTECODE_FLAG },
    { "socket", NULL, 4979776, 16084, NUITKA_BYTECODE_FLAG },
    { "sqlite3", NULL, 4995860, 154, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "sqlite3.dbapi2", NULL, 4996014, 2682, NUITKA_BYTECODE_FLAG },
    { "sqlite3.dump", NULL, 4998696, 2057, NUITKA_BYTECODE_FLAG },
    { "sre", NULL, 5000753, 509, NUITKA_BYTECODE_FLAG },
    { "ssl", NULL, 5001262, 32402, NUITKA_BYTECODE_FLAG },
    { "stat", NULL, 5033664, 2723, NUITKA_BYTECODE_FLAG },
    { "statvfs", NULL, 5036387, 610, NUITKA_BYTECODE_FLAG },
    { "stringold", NULL, 5036997, 12487, NUITKA_BYTECODE_FLAG },
    { "subprocess", NULL, 5049484, 32282, NUITKA_BYTECODE_FLAG },
    { "sunau", NULL, 5081766, 18290, NUITKA_BYTECODE_FLAG },
    { "sunaudio", NULL, 5100056, 1969, NUITKA_BYTECODE_FLAG },
    { "symbol", NULL, 5102025, 3014, NUITKA_BYTECODE_FLAG },
    { "symtable", NULL, 5105039, 11678, NUITKA_BYTECODE_FLAG },
    { "sysconfig", NULL, 5116717, 18805, NUITKA_BYTECODE_FLAG },
    { "tabnanny", NULL, 5135522, 8199, NUITKA_BYTECODE_FLAG },
    { "tarfile", NULL, 5143721, 75786, NUITKA_BYTECODE_FLAG },
    { "telnetlib", NULL, 5219507, 23080, NUITKA_BYTECODE_FLAG },
    { "tempfile", NULL, 5242587, 20228, NUITKA_BYTECODE_FLAG },
    { "test", NULL, 5262815, 114, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "test.pystone", NULL, 5262929, 7981, NUITKA_BYTECODE_FLAG },
    { "textwrap", NULL, 5270910, 11996, NUITKA_BYTECODE_FLAG },
    { "this", NULL, 5282906, 1210, NUITKA_BYTECODE_FLAG },
    { "threading", NULL, 5284116, 42422, NUITKA_BYTECODE_FLAG },
    { "timeit", NULL, 5326538, 12149, NUITKA_BYTECODE_FLAG },
    { "tkColorChooser", NULL, 5338687, 1402, NUITKA_BYTECODE_FLAG },
    { "tkCommonDialog", NULL, 5340089, 1496, NUITKA_BYTECODE_FLAG },
    { "tkFileDialog", NULL, 5341585, 5121, NUITKA_BYTECODE_FLAG },
    { "tkFont", NULL, 5346706, 7124, NUITKA_BYTECODE_FLAG },
    { "tkMessageBox", NULL, 5353830, 3864, NUITKA_BYTECODE_FLAG },
    { "tkSimpleDialog", NULL, 5357694, 9053, NUITKA_BYTECODE_FLAG },
    { "toaiff", NULL, 5366747, 3090, NUITKA_BYTECODE_FLAG },
    { "token", NULL, 5369837, 3798, NUITKA_BYTECODE_FLAG },
    { "tokenize", NULL, 5373635, 14588, NUITKA_BYTECODE_FLAG },
    { "trace", NULL, 5388223, 22716, NUITKA_BYTECODE_FLAG },
    { "traceback", NULL, 5410939, 11631, NUITKA_BYTECODE_FLAG },
    { "ttk", NULL, 5422570, 62344, NUITKA_BYTECODE_FLAG },
    { "tty", NULL, 5484914, 1303, NUITKA_BYTECODE_FLAG },
    { "turtle", NULL, 5486217, 139105, NUITKA_BYTECODE_FLAG },
    { "unittest", NULL, 5625322, 2954, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "unittest.case", NULL, 5628276, 40207, NUITKA_BYTECODE_FLAG },
    { "unittest.loader", NULL, 5668483, 11295, NUITKA_BYTECODE_FLAG },
    { "unittest.main", NULL, 5679778, 7970, NUITKA_BYTECODE_FLAG },
    { "unittest.result", NULL, 5687748, 7869, NUITKA_BYTECODE_FLAG },
    { "unittest.runner", NULL, 5695617, 7581, NUITKA_BYTECODE_FLAG },
    { "unittest.signals", NULL, 5703198, 2753, NUITKA_BYTECODE_FLAG },
    { "unittest.suite", NULL, 5705951, 10460, NUITKA_BYTECODE_FLAG },
    { "unittest.util", NULL, 5716411, 4494, NUITKA_BYTECODE_FLAG },
    { "urllib", NULL, 5720905, 50958, NUITKA_BYTECODE_FLAG },
    { "urllib2", NULL, 5771863, 47019, NUITKA_BYTECODE_FLAG },
    { "urlparse", NULL, 5818882, 15038, NUITKA_BYTECODE_FLAG },
    { "user", NULL, 5833920, 1714, NUITKA_BYTECODE_FLAG },
    { "uu", NULL, 5835634, 4294, NUITKA_BYTECODE_FLAG },
    { "uuid", NULL, 5839928, 23095, NUITKA_BYTECODE_FLAG },
    { "warnings", NULL, 5863023, 13456, NUITKA_BYTECODE_FLAG },
    { "wave", NULL, 5876479, 19903, NUITKA_BYTECODE_FLAG },
    { "weakref", NULL, 5896382, 16327, NUITKA_BYTECODE_FLAG },
    { "webbrowser", NULL, 5912709, 19652, NUITKA_BYTECODE_FLAG },
    { "whichdb", NULL, 5932361, 2229, NUITKA_BYTECODE_FLAG },
    { "wsgiref", NULL, 5934590, 719, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wsgiref.handlers", NULL, 5935309, 16172, NUITKA_BYTECODE_FLAG },
    { "wsgiref.headers", NULL, 5951481, 7429, NUITKA_BYTECODE_FLAG },
    { "wsgiref.simple_server", NULL, 5958910, 6199, NUITKA_BYTECODE_FLAG },
    { "wsgiref.util", NULL, 5965109, 5961, NUITKA_BYTECODE_FLAG },
    { "wsgiref.validate", NULL, 5971070, 16767, NUITKA_BYTECODE_FLAG },
    { "xdrlib", NULL, 5987837, 9810, NUITKA_BYTECODE_FLAG },
    { "xml", NULL, 5997647, 1068, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom", NULL, 5998715, 6427, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom.NodeFilter", NULL, 6005142, 1112, NUITKA_BYTECODE_FLAG },
    { "xml.dom.domreg", NULL, 6006254, 3293, NUITKA_BYTECODE_FLAG },
    { "xml.dom.expatbuilder", NULL, 6009547, 32810, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minicompat", NULL, 6042357, 3394, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minidom", NULL, 6045751, 65141, NUITKA_BYTECODE_FLAG },
    { "xml.dom.pulldom", NULL, 6110892, 12986, NUITKA_BYTECODE_FLAG },
    { "xml.dom.xmlbuilder", NULL, 6123878, 15222, NUITKA_BYTECODE_FLAG },
    { "xml.etree", NULL, 6139100, 119, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.etree.ElementInclude", NULL, 6139219, 1951, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementPath", NULL, 6141170, 7560, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementTree", NULL, 6148730, 34966, NUITKA_BYTECODE_FLAG },
    { "xml.etree.cElementTree", NULL, 6183696, 166, NUITKA_BYTECODE_FLAG },
    { "xml.parsers", NULL, 6183862, 304, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.parsers.expat", NULL, 6184166, 277, NUITKA_BYTECODE_FLAG },
    { "xml.sax", NULL, 6184443, 3679, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.sax._exceptions", NULL, 6188122, 6127, NUITKA_BYTECODE_FLAG },
    { "xml.sax.expatreader", NULL, 6194249, 15081, NUITKA_BYTECODE_FLAG },
    { "xml.sax.handler", NULL, 6209330, 12970, NUITKA_BYTECODE_FLAG },
    { "xml.sax.saxutils", NULL, 6222300, 14685, NUITKA_BYTECODE_FLAG },
    { "xml.sax.xmlreader", NULL, 6236985, 19078, NUITKA_BYTECODE_FLAG },
    { "xmllib", NULL, 6256063, 26732, NUITKA_BYTECODE_FLAG },
    { "xmlrpclib", NULL, 6282795, 43824, NUITKA_BYTECODE_FLAG },
    { "zipfile", NULL, 6326619, 42007, NUITKA_BYTECODE_FLAG },
    { NULL, NULL, 0, 0, 0 }
};

void setupMetaPathBasedLoader( void )
{
    static bool init_done = false;

    if ( init_done == false )
    {
        registerMetaPathBasedUnfreezer( meta_path_loader_entries );
        init_done = true;
    }
}
