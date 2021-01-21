#pragma once

/*
This .h file is only intended to be included by client applications. So Hare will never include this.
For use by Hare applications.
*/

#include "Hare/Core/Log.h"
#include "Hare/Core/Layer.h"
#include "Hare/Core/Application.h"

#include "Hare/Core/TimeStep.h"

#include "Hare/Core/Input.h"
#include "Hare/Core/Keycodes.h"
#include "Hare/Core/MouseButtonCodes.h"
#include "Hare/Renderer/OrthographicCameraController.h"

#include "Hare/ImGui/ImGuiLayer.h"

// --- Renderer ----------------
#include "Hare/Renderer/Renderer.h"
#include "Hare/Renderer/RenderCommand.h"

#include "Hare/Renderer/Buffer.h"
#include "Hare/Renderer/Shader.h"
#include "Hare/Renderer/Texture.h"
#include "Hare/Renderer/VertexArray.h"

#include "Hare/Renderer/OrthographicCamera.h"
// -------------------------------------------

// ---Entry Point-----------
#include "Hare/Core/EntryPoint.h"
// -------------------------