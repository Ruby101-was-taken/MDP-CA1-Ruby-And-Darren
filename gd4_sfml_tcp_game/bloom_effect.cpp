#include "bloom_effect.hpp"
#include "shader_types.hpp"

BloomEffect::BloomEffect()
{
	shaders_.Load(ShaderTypes::kBrightnessPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/Brightness.frag");
	shaders_.Load(ShaderTypes::kDownSamplePass, "Media/Shaders/Fullpass.vert", "Media/Shaders/DownSample.frag");
	shaders_.Load(ShaderTypes::kGaussianBlurPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/GuassianBlur.frag");
	shaders_.Load(ShaderTypes::kAddPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/Add.frag");
}

void BloomEffect::Apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
	PrepareTextures(input.getSize());

	FilterBright(input, brightness_texture_);

	Downsample(brightness_texture_, firstpass_textures_[0]);
	BlurMultipass(firstpass_textures_);

	Downsample(firstpass_textures_[0], secondpass_textures_[0]);
	BlurMultipass(secondpass_textures_);

	Add(firstpass_textures_[0], secondpass_textures_[0], firstpass_textures_[1]);
	firstpass_textures_[1].display();
	Add(input, firstpass_textures_[1], output);
}

void BloomEffect::PrepareTextures(sf::Vector2u size)
{
	if (brightness_texture_.getSize() != size) {
		brightness_texture_.resize({ size.x, size.y });
		brightness_texture_.setSmooth(true);

		firstpass_textures_[0].resize({ size.x / 2, size.y / 2 });
		firstpass_textures_[0].setSmooth(true);
		firstpass_textures_[1].resize({ size.x / 2, size.y / 2 });
		firstpass_textures_[1].setSmooth(true);

		secondpass_textures_[0].resize({ size.x / 4, size.y / 4 });
		secondpass_textures_[0].setSmooth(true);
		secondpass_textures_[1].resize({size.x / 4, size.y / 4 }); 
		secondpass_textures_[1].setSmooth(true);
	}
}

void BloomEffect::FilterBright(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	sf::Shader& brightness = shaders_.Get(ShaderTypes::kBrightnessPass);

	brightness.setUniform("source", input.getTexture());
	ApplyShader(brightness, output);
	output.display();
}

void BloomEffect::BlurMultipass(RenderTextureArray& renderTextures)
{
	sf::Vector2u textureSize = renderTextures[0].getSize();

	for (std::size_t count = 0; count < 2; ++count)
	{
		Blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
		Blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
	}
}

void BloomEffect::Blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
	sf::Shader& gaussianBlur = shaders_.Get(ShaderTypes::kGaussianBlurPass);

	gaussianBlur.setUniform("source", input.getTexture());
	gaussianBlur.setUniform("offsetFactor", offsetFactor);
	ApplyShader(gaussianBlur, output);
	output.display();
}

void BloomEffect::Downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	sf::Shader& downSampler = shaders_.Get(ShaderTypes::kDownSamplePass);

	downSampler.setUniform("source", input.getTexture());
	downSampler.setUniform("sourceSize", sf::Vector2f(input.getSize()));
	ApplyShader(downSampler, output);
	output.display();
}

void BloomEffect::Add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
{
	sf::Shader& adder = shaders_.Get(ShaderTypes::kAddPass);

	adder.setUniform("source", source.getTexture());
	adder.setUniform("bloom", bloom.getTexture());
	ApplyShader(adder, output);
}