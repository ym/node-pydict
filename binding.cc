// binding.cc
#include <node.h>
#include <v8.h>
#include <python2.6/Python.h>

using namespace v8;

Handle<Value> PyDict2JSON(const Arguments& args) {
    HandleScope scope;
    
    if(args.Length() < 1 || !args[0]->IsString()) {
        return ThrowException(Exception::Error(String::New("死鬼~让你买的避孕套呢~")));
    }
    
    /* Import AST */
    
    Py_Initialize();
    
    PyObject* PythonAST = PyImport_Import(PyString_FromString("ast"));
    
    PyObject* PythonAST_Eval = PyDict_GetItemString(PyModule_GetDict(PythonAST), "literal_eval");
    
    if (!PyCallable_Check(PythonAST_Eval)) {
        Py_Finalize();
        
        return ThrowException(Exception::Error(String::New("嘤嘤嘤伦家不会解析字符串了肿么办！")));
    }
    
    /* Arguments for ast.literal_eval */
    
    PyObject* PythonAST_Args = PyTuple_New(1);
    
    PyTuple_SetItem(PythonAST_Args, 0, PyString_FromString(*String::Utf8Value(args[0]->ToString())));
    
    /* Parse string to PyDict */

    PyObject* dict = PyObject_CallObject(PythonAST_Eval, PythonAST_Args);
    
    if(dict == NULL) {
        Py_Finalize();
        
        return ThrowException(Exception::Error(String::New("你买的是什么避孕套呀怎么破的！说了要杜蕾斯的！不要杜拉斯的！")));
    }
    
    /* Import JSON */
    
    PyObject* PythonJSON = PyImport_Import(PyString_FromString("json"));
    
    PyObject* PythonJSON_Dumps = PyDict_GetItemString(PyModule_GetDict(PythonJSON), "dumps");
    
    if (!PyCallable_Check(PythonJSON_Dumps)) {
        Py_Finalize();
        
        return ThrowException(Exception::Error(String::New("诶~我的内内呢~")));
    }
    
    /* Arguments for json.dumps */
    
    PyObject* PythonJSON_Args = PyTuple_New(1);
    
    PyTuple_SetItem(PythonJSON_Args, 0, dict);
    
    /* Generate JSON */
    
    PyObject* result_json = PyObject_CallObject(PythonJSON_Dumps, PythonJSON_Args);
    
    /* Convert to String */
    
    char* native_string = PyString_AsString(result_json);
    
    /* Clean Up */
    
    Py_Finalize();
    
    return scope.Close(String::New(native_string));
}

void toJSON(Handle<Object> target) {
    // to JSON
    target->Set(String::NewSymbol("toJSON"),
                FunctionTemplate::New(PyDict2JSON)->GetFunction());
    
}
NODE_MODULE(binding, toJSON)