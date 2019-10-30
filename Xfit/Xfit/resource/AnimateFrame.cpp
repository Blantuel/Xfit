#include "AnimateFrame.h"
#include "../system/System.h"

AnimateFrame::AnimateFrame():uv(System::defaultUV),frame(nullptr),vertex(System::defaultVertex2D) {}
AnimateFrame::AnimateFrame(Frame* _frame, Vertex* _vertex, Vertex* _uv):frame(_frame),vertex(_vertex),uv(_uv) {}