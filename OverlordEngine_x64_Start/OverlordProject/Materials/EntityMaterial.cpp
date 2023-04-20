#include "stdafx.h"
#include "EntityMaterial.h"

EntityMaterial::EntityMaterial() :
	Material<EntityMaterial>(L"Effects/PosNormCol3D.fx")

{
}

void EntityMaterial::InitializeEffectVariables()
{
}
