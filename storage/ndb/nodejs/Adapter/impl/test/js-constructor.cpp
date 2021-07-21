/*
   Copyright (c) 2013, 2021, Oracle and/or its affiliates.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL hereby grant you an additional
   permission to link the program and your derivative works with the
   separately licensed software that they have included with MySQL.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include "node.h"

using namespace v8;

/*
HandleScope scope;

 * A FunctionTemplate can have properties, these properties are added to the
 * function object when it is created.

 * A FunctionTemplate has a corresponding instance template which is
 * used to create object instances when the function is used as a
 * constructor. Properties added to the instance template are added to
 * each object instance.

    Local<FunctionTemplate> t = FunctionTemplate::New();
    t->Set("func_property", Number::New(1));

    Local<v8::ObjectTemplate> proto_t = t->PrototypeTemplate();
    //proto_t->Set("proto_method", v8::FunctionTemplate::New(InvokeCallback));
    proto_t->Set("proto_const", v8::Number::New(2));

    v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();
    //instance_t->SetAccessor("instance_accessor", InstanceAccessorCallback);
    //instance_t->SetNamedPropertyHandler(PropertyHandlerCallback, ...);
    instance_t->Set("instance_property", Number::New(3));

    v8::Local<v8::Function> function = t->GetFunction();
    v8::Local<v8::Object> instance = function->NewInstance();

    target->Set(String::NewSymbol("jscons"), t);
*/


Handle<Value> pGetter(Local<String> property,
                     const AccessorInfo & info) 
{
  /* info has Data(), This(), and Holder() */
  HandleScope scope;
  return scope.Close(info.Data());
}                    

void pSetter(Local<String> property,
             Local<Value> value,
             const AccessorInfo& info) 
{

}



/*  
  var builder=require("./jscons").builder

   calling builder("foo", 66) 
     should return a constructor
     for objects 
     with a property named "foo"
     whose value is 66
*/
Handle<Value> theConstructor(const Arguments &args) {
  HandleScope scope;

  if(args.IsConstructCall()) {
  }
  else {
    ThrowException(Exception::Error(String::New("must be a called as constructor")));
  }
  return args.This();
}


Handle<Value> ConstructorBuilder(const Arguments &args) {
  HandleScope scope;
  Local<String> methodName = args[0]->ToString();
  Local<Value> magic = args[1];
  Local<FunctionTemplate> ft = FunctionTemplate::New();

  ft->SetCallHandler(theConstructor);
  ft->InstanceTemplate()->SetAccessor(methodName, pGetter, pSetter, magic);
  ft->InstanceTemplate()->Set(String::NewSymbol("isMappedNdbRecord"),
                              True(), DontEnum);
  return scope.Close(ft->GetFunction());
}


void initOnLoad(Handle<Object> target) {
  HandleScope scope;
  Local<FunctionTemplate> ft = FunctionTemplate::New();
  ft->SetCallHandler(ConstructorBuilder);
  
  target->Set(String::NewSymbol("builder"), ft->GetFunction());
}


/*  FINAL STEP.
    This macro associates the module name with its initializer function 
*/
NODE_MODULE(jscons, initOnLoad)
