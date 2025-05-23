#ifdef _WINDOWS

#include "Resources/Dx11/Dx11Texture.hpp"

#include <d3d11.h>
#include <stdio.h>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Modules/RenderModule.hpp"
#include "Render/Renderer.hpp"


static bool PadImageData(unsigned char*& imageData, int width, int height, int channels) {
	// Assuming you have the image data and width, height, and channels are set correctly
	if (channels != 3) { return false; }

	unsigned char* paddedImageData = new unsigned char[width * height * 4];  // 4 channels (RGBA)

	// Copy RGB to RGBA
	for (int i = 0; i < width * height; ++i) {
		int srcIndex = i * 3;         // Index in the original 3-channel image
		int destIndex = i * 4;        // Index in the new 4-channel image

		paddedImageData[destIndex] = imageData[srcIndex];     // Red
		paddedImageData[destIndex + 1] = imageData[srcIndex + 1]; // Green
		paddedImageData[destIndex + 2] = imageData[srcIndex + 2]; // Blue
		paddedImageData[destIndex + 3] = 255; // Alpha (fully opaque)
	}

	// Replace the original image data with the padded image
	stbi_image_free(imageData);
	imageData = paddedImageData;
	channels = 4;  // Update the channel count

	return true;
}


bool Dx11Texture::Load(const std::string& filePath) {
	return Initialize(filePath.c_str());
}

void Dx11Texture::Cleanup() {
	Shutdown();
}

bool Dx11Texture::Initialize(const char* path) {
	DeviceVariant deviceVariant = RenderModule::GetInstance()->renderer->GetDevice();
	ID3D11Device** ptrDevice = std::get_if<ID3D11Device*>(&deviceVariant);
	if (ptrDevice == nullptr) { throw std::runtime_error("Wrong API Device Variant"); }

	DeviceContextVariant deviceContextVariant = RenderModule::GetInstance()->renderer->GetDeviceContext();
	ID3D11DeviceContext** ptrDeviceContext = std::get_if<ID3D11DeviceContext*>(&deviceContextVariant);
	if (ptrDeviceContext == nullptr) { throw std::runtime_error("Wrong API Device Context Variant"); }

	return InitializeTexture(*ptrDevice, *ptrDeviceContext, path);
}

void Dx11Texture::Shutdown() {

	// Release the texture view resource.
	if (textureView != nullptr) {
		textureView->Release();
		textureView = nullptr;
	}

}

ShaderRessourceViewVariant Dx11Texture::GetTexture() {
	return textureView;
}

bool Dx11Texture::InitializeTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* path) {
	// Load image with stb_image
	int width_;
	int height_;
	int channels;
	unsigned char* imageData = stbi_load(path, &width_, &height_, &channels, 0);

	if (imageData == nullptr) { return false; }

	width = width_;
	height = height_;

	// Determine the texture format
	DXGI_FORMAT format;
	switch (channels) {
	case 1: format = DXGI_FORMAT_R8_UNORM; break;
	case 2: format = DXGI_FORMAT_R8G8_UNORM; break;
	case 3: format = DXGI_FORMAT_R8G8B8A8_UNORM; break; // Needs padding
	case 4: format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	default: stbi_image_free(imageData); return false;
	}

	bool padded = PadImageData(imageData, width, height, channels);

	// Prepare the texture data
	D3D11_TEXTURE2D_DESC texDesc = {
		.Width = width,
		.Height = height,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = format,
		.SampleDesc {
			.Count = 1,
			.Quality = 0
		},
		.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET,
		.CPUAccessFlags = 0,
		.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_GENERATE_MIPS
	};

	// Create the texture
	/*D3D11_SUBRESOURCE_DATA initData = {
		.pSysMem = imageData,
		.SysMemPitch = width * channels,
		.SysMemSlicePitch = 0
	};*/

	ID3D11Texture2D* texture2D = nullptr;
	HRESULT hr = device->CreateTexture2D(&texDesc, /*&initData*/ nullptr, &texture2D);
	if (FAILED(hr)) {
		stbi_image_free(imageData);
		return false;
	}

	unsigned int rowPitch = width * channels * sizeof(unsigned char);
	deviceContext->UpdateSubresource(texture2D, 0, nullptr, imageData, rowPitch, 0);

	// Create shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {
		.Format = texDesc.Format,
		.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D {
			.MostDetailedMip = 0,
			.MipLevels = 1
		}
	};

	hr = device->CreateShaderResourceView(texture2D, &srvDesc, &textureView);
	deviceContext->GenerateMips(textureView);

	texture2D->Release();  // Release the texture after creating the SRV
	if (!padded) { stbi_image_free(imageData); } // Free the image data
	else { delete[] imageData; }
}

#endif