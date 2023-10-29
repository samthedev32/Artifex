#include <Artifex/core/UI.hpp>

#include <Artifex/Engine.hpp>

namespace Artifex {

void UI::init(Engine *pEngine) {
  if (initialized)
    return;

  // TODO
  engine = pEngine;

  initialized = true;
}

void UI::deinit() {
  if (!initialized)
    return;

  // TODO

  initialized = false;
}

// bool UI::button(vec2 center, vec2 size, std::string name,
// uint16_t a,
//                 uint16_t b) {
//     // if (math::collide(cursor, center, size * ratio())) {
//     //     if (input[name] && key("mouse") == false) {
//     //         input[name] = false;
//     //         return true;
//     //     }
//     //     input[name] = key("mouse");
//     // } else
//     //     input[name] = false;

//     // rect(center, size, input[name] ? b : a);
//     return false;
// }

// bool UI::toggle(vec2 center, vec2 size, std::string name,
// uint16_t a,
//                 uint16_t b) {
//     // if (math::collide(cursor, center, size * ratio())) {
//     //     if (key("mouse") == true) {
//     //         if (input[name] > 0)
//     //             input[name] = 2;

//     //         if (input[name] <= 0)
//     //             input[name] = -2;
//     //     } else {
//     //         if (input[name] == 2)
//     //             input[name] = -1;

//     //         if (input[name] == -2)
//     //             input[name] = 1;
//     //     }
//     // } else {
//     //     if (input[name] > 0)
//     //         input[name] = 1;
//     //     if (input[name] <= 0)
//     //         input[name] = -1;
//     // }

//     // rect(center, size, input[name] > 0 ? b : a);

//     return false;
// }

// bool UI::touch(vec2 center, vec2 size, uint16_t a, uint16_t
// b) {
//     // if (key("mouse") && math::collide(cursor, center, size
//     * ratio())) {
//     //     rect(center, size, b);
//     //     return true;
//     // }

//     // rect(center, size, a);
//     return false;
// }

// vec2 UI::joystick(vec2 center, std::string name, float
// radius, float nob_radius,
//                   vec3 color, vec3 nob_color) {
//     circle(center, radius, 0.9f, color);

//     vec2 pos = center;

//     if (key("mouse")) {
//         if (cursor.distance(center) <= radius && input[name]
//         == false)
//             input[name] = true;

//         if (input[name])
//             pos = cursor;
//     } else
//         input[name] = false;

//     // Works - Don't touch it!
//     // WARNING: Possible Div by 0
//     vec2 direction = (pos - center).normalize() * radius;
//     float distance = math::clamp(pos.distance(center) /
//     radius, -1.0f, 1.0f);

//     circle(center + direction * distance * ratio(),
//     nob_radius, 0.0f,
//            nob_color);

//     vec2 a = pos - center;
//     a.x = math::map(a.x, -radius, radius, -1, 1);
//     a.y = math::map(a.y, -radius, radius, -1, 1);

//     return math::clamp(a, vec2(-1.0f, -1), vec2(1.0f, 1.0f));
// }

} // namespace Artifex