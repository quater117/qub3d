/*
*	 Copyright (C) 2018 Qub³d Engine Group.
*	 All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification,
*  are permitted provided that the following conditions are met:
*
*  1. Redistributions of source code must retain the above copyright notice, this
*  list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright notice,
*  this list of conditions and the following disclaimer in the documentation and/or
*  other materials provided with the distribution.
*
*  3. Neither the name of the copyright holder nor the names of its contributors
*  may be used to endorse or promote products derived from this software without
*  specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
*  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
*  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file window.hpp
 * @brief Function declarations for the qub3d::graphics::Window class
 */

#pragma once

#include <SDL.h>

#include <string>
#include <functional>
#include <vector>

namespace qub3d
{

    namespace graphics
    {

        /**
         * @brief Thin Abstraction Layer, to provide easier interface for Window related information.
         *
         * @details Thin abstraction layer over SDL_Window* -> provides a modern C++ interface to handle events and
         * handle any boilerplate window/OpenGL context code.
         */
        class Window
        {
        public:
            /**
             * @brief Creates and shows the window along with a valid current OpenGL context (with GLEW initalized).
             *
             * @details Creates a OpenGL window with the specified parameters in the centre of the window, along with a GLEW initalized
             *          OpenGL context (which is current). Also will initalize ImGui.
             *
             * @param[in] title The title of the window
             * @param[in] w		The width of the window (in screen coordinates)
             * @param[in] h		The height of the window (in screen coordinates)
             */
            Window( const std::string& title, int w, int h );

            /**
             * @brief Free's any resources that have been allocated by this object.
             *
             * @details Destroy the SDL_Window* and the OpenGL context belonging to this window. Invalidates any results of `getSDLWindow`.
             */
            ~Window();

            /**
             * @brief Returns the SDL_Window pointer for this current window.
             *
             * @details Exposes the internal SDL_Window pointer for use when interacting directly with OpenGL.
             *          This pointer is valid for the lifetime of the Window instance, and is
             *          not guaranteed to be valid after the destructor of this class (Window) has been called.
             *
             *          Do not call SDL_DestroyWindow on the returned pointer (or attempt to delete/free it) as this is handled
             *          internally.
             *
             * @returns The pointer to the SDL_Window struct.
             */
            inline SDL_Window *getSDLWindow() const { return m_window; }

            /**
             * @brief Finalises a frame and swaps the OpenGL buffers.
             *
             * @details Will render the current ImGUI state to the screen (preserving any prior OpenGL state),
             *          and then swaps the OpenGL buffers for this window (if double buffering is enabled, which it is by default).
             *          Should be called at the end of the game loop (or wherever rendering is finished for that frame).
             */
            void swapBuffers();

            /**
             * @brief Polls the SDL event queue at the start of a frame.
             *
             * @details Poll's the SDL event queue and will execute any event handlers that have been added
             *          via `addEventHandler`.
             *          Must be called at the start of the game loop.
             */
            void pollEvents();

            /**
             * @brief Use this function to tell if the window is currently running.
             * @returns true when the window is running, false when the user decides to terminate the window.
             */
            inline bool isRunning() const { return m_isRunning; }

        private:
            SDL_Window * m_window;
            SDL_GLContext m_context;
            bool m_isRunning;

        };

    } // namespace graphics
} // namespace qub3d
