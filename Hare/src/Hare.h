#pragma once

// For use by Hare applications.
// This .h file is only intended to be included by client applications. So Hare will never include this.
#include "Hare/Core/Definitions.h"
#include "Hare/Core/Log.h"
#include "Hare/Core/Layer.h"
#include "Hare/Core/Application.h"

#include "Hare/Core/TimeStep.h"

#include "Hare/Core/Input.h"
#include "Hare/Core/KeyCodes.h"
#include "Hare/Core/MouseButtonCodes.h"
#include "Hare/Renderer/OrthographicCameraController.h"

#include "Hare/ImGui/ImGuiLayer.h"

#include "Hare/Scene/Scene.h"
#include "Hare/Scene/Entity.h"
#include "Hare/Scene/ScriptableEntity.h"
#include "Hare/Scene/Components.h"

// --- Renderer ----------------
#include "Hare/Renderer/Renderer.h"
#include "Hare/Renderer/Renderer2D.h"
#include "Hare/Renderer/RenderCommand.h"

#include "Hare/Renderer/Buffer.h"
#include "Hare/Renderer/Shader.h"
#include "Hare/Renderer/Texture.h"
#include "Hare/Renderer/Framebuffer.h"
#include "Hare/Renderer/SubTexture2D.h"
#include "Hare/Renderer/VertexArray.h"

#include "Hare/Renderer/OrthographicCamera.h"
// -------------------------------------------