//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#include "pxr/usd/usdGeom/imageable.h"
#include "pxr/usd/usd/schemaBase.h"

#include "pxr/usd/sdf/primSpec.h"

#include "pxr/usd/usd/pyConversions.h"
#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyResultConversions.h"
#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

#include <boost/python.hpp>

#include <string>

using namespace boost::python;

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

#define WRAP_CUSTOM                                                     \
    template <class Cls> static void _CustomWrapCode(Cls &_class)

// fwd decl.
WRAP_CUSTOM;

        
static UsdAttribute
_CreateVisibilityAttr(UsdGeomImageable &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateVisibilityAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Token), writeSparsely);
}
        
static UsdAttribute
_CreatePurposeAttr(UsdGeomImageable &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreatePurposeAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Token), writeSparsely);
}

static std::string
_Repr(const UsdGeomImageable &self)
{
    std::string primRepr = TfPyRepr(self.GetPrim());
    return TfStringPrintf(
        "UsdGeom.Imageable(%s)",
        primRepr.c_str());
}

} // anonymous namespace

void wrapUsdGeomImageable()
{
    typedef UsdGeomImageable This;

    class_<This, bases<UsdTyped> >
        cls("Imageable");

    cls
        .def(init<UsdPrim>(arg("prim")))
        .def(init<UsdSchemaBase const&>(arg("schemaObj")))
        .def(TfTypePythonClass())

        .def("Get", &This::Get, (arg("stage"), arg("path")))
        .staticmethod("Get")

        .def("GetSchemaAttributeNames",
             &This::GetSchemaAttributeNames,
             arg("includeInherited")=true,
             return_value_policy<TfPySequenceToList>())
        .staticmethod("GetSchemaAttributeNames")

        .def("_GetStaticTfType", (TfType const &(*)()) TfType::Find<This>,
             return_value_policy<return_by_value>())
        .staticmethod("_GetStaticTfType")

        .def(!self)

        
        .def("GetVisibilityAttr",
             &This::GetVisibilityAttr)
        .def("CreateVisibilityAttr",
             &_CreateVisibilityAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetPurposeAttr",
             &This::GetPurposeAttr)
        .def("CreatePurposeAttr",
             &_CreatePurposeAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))

        
        .def("GetProxyPrimRel",
             &This::GetProxyPrimRel)
        .def("CreateProxyPrimRel",
             &This::CreateProxyPrimRel)
        .def("__repr__", ::_Repr)
    ;

    _CustomWrapCode(cls);
}

// ===================================================================== //
// Feel free to add custom code below this line, it will be preserved by 
// the code generator.  The entry point for your custom code should look
// minimally like the following:
//
// WRAP_CUSTOM {
//     _class
//         .def("MyCustomMethod", ...)
//     ;
// }
//
// Of course any other ancillary or support code may be provided.
// 
// Just remember to wrap code in the appropriate delimiters:
// 'namespace {', '}'.
//
// ===================================================================== //
// --(BEGIN CUSTOM CODE)--

#include "pxr/base/tf/pyObjWrapper.h"

namespace {

static TfPyObjWrapper
_ComputeProxyPrim(UsdGeomImageable const &self)
{
    UsdPrim  renderPrim, proxyPrim;
    
    if (self){
        proxyPrim = self.ComputeProxyPrim(&renderPrim);
        if (proxyPrim){
            return TfPyObjWrapper(boost::python::make_tuple(proxyPrim, 
                                                            renderPrim));
        }
    }
    return TfPyObjWrapper();
}

static std::string _GetPurpose(const UsdGeomImageable::PurposeInfo &purposeInfo)
{
    return purposeInfo.purpose;
}

static void _SetPurpose(UsdGeomImageable::PurposeInfo &purposeInfo, 
                        const std::string &purpose)
{
    purposeInfo.purpose = TfToken(purpose);
}

static bool _IsValidPurposeInfo(const UsdGeomImageable::PurposeInfo &purposeInfo)
{
    return bool(purposeInfo);
}

WRAP_CUSTOM {

    _class
        .def("GetOrderedPurposeTokens",
             &UsdGeomImageable::GetOrderedPurposeTokens,
             return_value_policy<TfPySequenceToList>())
        .staticmethod("GetOrderedPurposeTokens")

        .def("ComputeVisibility", 
             &UsdGeomImageable::ComputeVisibility,
             arg("time")=UsdTimeCode::Default())

        .def("GetPurposeVisibilityAttr",
             &UsdGeomImageable::GetPurposeVisibilityAttr,
             (arg("purpose") = UsdGeomTokens->default_))
        .def("ComputeEffectiveVisibility",
             &UsdGeomImageable::ComputeEffectiveVisibility,
             (arg("purpose") = UsdGeomTokens->default_,
              arg("time") = UsdTimeCode::Default()))

        .def("ComputePurpose", 
             (TfToken (UsdGeomImageable::*)() const)
                &UsdGeomImageable::ComputePurpose)
        .def("ComputePurposeInfo",
             (UsdGeomImageable::PurposeInfo (UsdGeomImageable::*)() const)
                &UsdGeomImageable::ComputePurposeInfo)
        .def("ComputePurposeInfo",
             (UsdGeomImageable::PurposeInfo (UsdGeomImageable::*)(
                const UsdGeomImageable::PurposeInfo &) const)
                &UsdGeomImageable::ComputePurposeInfo,
             arg("parentPurposeInfo"))

        .def("ComputeProxyPrim", &_ComputeProxyPrim,
            "Returns None if neither this prim nor any of its ancestors "
            "has a valid renderProxy prim.  Otherwise, returns a tuple of "
            "(proxyPrim, renderPrimWithAuthoredProxyPrimRel)")
        .def("SetProxyPrim", 
             (bool (UsdGeomImageable::*)(const UsdPrim &) const)
             &UsdGeomImageable::SetProxyPrim, 
             arg("proxy"))
        .def("SetProxyPrim", 
             (bool (UsdGeomImageable::*)(const UsdSchemaBase &) const)
             &UsdGeomImageable::SetProxyPrim, 
             arg("proxy"))
        .def("MakeVisible", &UsdGeomImageable::MakeVisible, 
            arg("time")=UsdTimeCode::Default())
        .def("MakeInvisible", &UsdGeomImageable::MakeInvisible, 
            arg("time")=UsdTimeCode::Default())
        .def("ComputeWorldBound", &UsdGeomImageable::ComputeWorldBound,
             (arg("time"), arg("purpose1")=TfToken(), arg("purpose2")=TfToken(),
              arg("purpose3")=TfToken(), arg("purpose4")=TfToken()))
        .def("ComputeLocalBound", &UsdGeomImageable::ComputeLocalBound,
             (arg("time"), arg("purpose1")=TfToken(), arg("purpose2")=TfToken(),
              arg("purpose3")=TfToken(), arg("purpose4")=TfToken()))
        .def("ComputeUntransformedBound",
             &UsdGeomImageable::ComputeUntransformedBound,
             (arg("time"), arg("purpose1")=TfToken(), arg("purpose2")=TfToken(),
              arg("purpose3")=TfToken(), arg("purpose4")=TfToken()))
        .def("ComputeLocalToWorldTransform",
             &UsdGeomImageable::ComputeLocalToWorldTransform, (arg("time")))
        .def("ComputeParentToWorldTransform",
             &UsdGeomImageable::ComputeParentToWorldTransform, (arg("time")))
        ;

        {
            scope s = _class;
            class_<UsdGeomImageable::PurposeInfo>("PurposeInfo")
                .def(init<>())
                .def(init<const TfToken &, bool>())
                .def("__bool__", &_IsValidPurposeInfo)
                .def(self == self)
                .def(self != self)
                .add_property("purpose", &_GetPurpose, &_SetPurpose)
                .def_readwrite("isInheritable",
                               &UsdGeomImageable::PurposeInfo::isInheritable)
                .def("GetInheritablePurpose", 
                     make_function(
                         &UsdGeomImageable::PurposeInfo::GetInheritablePurpose,
                         return_value_policy<return_by_value>()))
            ;
        }
}

} // anonymous namespace 
