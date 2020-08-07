#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP
#include "GeneralModules/inter.h"

namespace Graphics{
    class FrameBuffer{
        public:
         uint id;
         FrameBuffer();
         ~FrameBuffer();
         void activate() const;
         void deactivate() const;
    };
    }  // namespace Graphics

#endif
