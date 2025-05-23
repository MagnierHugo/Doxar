#pragma once

#include <variant>


// Add more types when adding RenderingAPI
using DeviceVariant = std::variant<struct ID3D11Device*>;
using DeviceContextVariant = std::variant<struct ID3D11DeviceContext*>;
using ShaderRessourceViewVariant = std::variant<struct ID3D11ShaderResourceView*>;
using Texture2DVariant = std::variant<struct ID3D11Texture2D*>;