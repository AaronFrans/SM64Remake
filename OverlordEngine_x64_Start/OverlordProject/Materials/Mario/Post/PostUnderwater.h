#pragma once
class PostUnderwater : public PostProcessingMaterial
{
public:
	PostUnderwater();
	~PostUnderwater() override = default;
	PostUnderwater(const PostUnderwater& other) = delete;
	PostUnderwater(PostUnderwater&& other) noexcept = delete;
	PostUnderwater& operator=(const PostUnderwater& other) = delete;
	PostUnderwater& operator=(PostUnderwater&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};