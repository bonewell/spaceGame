#include "colorschema.hpp"

#include <cstdlib>

constexpr auto kColorModificationDelay = 10ms;

ColorSchema::ColorSchema(primitive::Color a, primitive::Color b)
    : color_a_(a), color_b_(b) {}

void ColorSchema::update(){
    if (primitive::now() <= change_colorchema_delay_) { return; }
    change_colorchema_delay_ = primitive::delay(kColorModificationDelay);

    if (flag_){
        if (color_mix_ >= 1){ flag_ = !flag_; return;}
        color_mix_ += 0.01;
        color_.red = color_a_.red * (1-color_mix_) + color_b_.red * (color_mix_);
        color_.green = color_a_.green * (1-color_mix_) + color_b_.green * (color_mix_);
        color_.blue = color_a_.blue * (1-color_mix_) + color_b_.blue * (color_mix_);
    } else {
        if (color_mix_ <= 0){ flag_ = !flag_; return;}
        color_mix_ -= 0.01;
        color_.red = color_a_.red * (1-color_mix_) + color_b_.red * (color_mix_);
        color_.green = color_a_.green * (1-color_mix_) + color_b_.green * (color_mix_);
        color_.blue = color_a_.blue * (1-color_mix_) + color_b_.blue * (color_mix_);
    }
}

void ColorSchema::update(double current_a){
    if (primitive::now() <= change_colorchema_delay_) { return; }
    change_colorchema_delay_ = primitive::delay(kColorModificationDelay);

    color_.red = color_a_.red + std::abs(color_a_.red - color_b_.red) * current_a;
    color_.green = color_a_.green + std::abs(color_a_.green - color_b_.green) * current_a;
    color_.blue = color_a_.blue + std::abs(color_a_.blue - color_b_.blue) * current_a;
}
