//
// This file is part of the NX Project
//
// Copyright (c) 2014 Leander Beernaert
//
// NX Project is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// NX Project is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with NX. If not, see <http://www.gnu.org/licenses/>.
//
#ifndef __NX_ENTITYBASE_H__
#define __NX_ENTITYBASE_H__

#include "nx/hdl/nxhdl.h"
namespace nx
{

enum EntityFlag
{
    kEntityFlagDirty = NX_BIT(0),
    kEntityFlagStatic = NX_BIT(1),
    kEntityFlagEnabled = NX_BIT(2),
    kEntityFlagVisibile = NX_BIT(3)
};

class NXTransform;
class NXAABB;
class NXComponentBase;
class NXSceneBase;
class NXEntityBase
{
public:

    typedef std::vector<NXHdl> ChildrenVec_t;
    typedef std::vector<NXComponentBase*> ComponentVec_t;

    virtual ~NXEntityBase();

    const char* name() const;

    void setName(const char* name);

    nx_u32 entityType() const;

    NXSceneBase* owner();

    NXTransform* localTransform();

    NXTransform* worldTransform();

    NXAABB* localBV();

    NXAABB* fullBV();

    const ChildrenVec_t& children() const;

    const ComponentVec_t& components() const;

    bool addChild(const NXHdl hdl);

    bool containsChild(const NXHdl hdl) const;

    void removeChild(const NXHdl hdl);

    bool addComponent(const NXComponentBase* pComponent);

    bool containsComponent(const nx_u32 componentType) const;

    bool containsComponent(const NXComponentBase* pComponent) const;

    void removeComponent(const nx_u32 componentType);

    NXComponentBase* removeComponentWithoutDelete(const nx_u32 componentType);

    NXComponentBase* component(const nx_u32 componentType);

    void updateTransforms();

    void updateComponents(const double seconds);

protected:
    NXEntityBase(const nx_u32 entityType);

private:
    NX_CPP_NO_COPY(NXEntityBase);

protected:
    friend class NXSceneBase;

    NXSceneBase* _pOwner;
    NXTransform* _pLocalTransform;
    NXTransform* _pWorldTransform;
    NXAABB* _pLocalBV;
    NXAABB* _pFullBV;

    NXHdl _parent;
    const nx_u32 _entityType;

    NXString _name;

    ChildrenVec_t _children;
    ComponentVec_t _components;

    nx_u32 _state;
};
}

#endif
