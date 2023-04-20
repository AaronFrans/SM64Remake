#pragma once

class EntityMaterial final : public Material<EntityMaterial>
{
public:
	EntityMaterial();
	~EntityMaterial() override = default;

	EntityMaterial(const EntityMaterial& other) = delete;
	EntityMaterial(EntityMaterial&& other) noexcept = delete;
	EntityMaterial& operator=(const EntityMaterial& other) = delete;
	EntityMaterial& operator=(EntityMaterial&& other) noexcept = delete;


protected:
	void InitializeEffectVariables() override;

private:
};
