#ifndef COLORSCHEMA_H
#define COLORSCHEMA_H

#include <initializer_list>
#include <vector>

#include "primitive/color.hpp"
#include "primitive/time.hpp"

class ColorSchema {
    const primitive::Color color_a_;
    const primitive::Color color_b_;
    primitive::Color color_;
    primitive::Time change_colorchema_delay_;
    double color_mix_{1.0};
    bool flag_{false};

public:
    ColorSchema(primitive::Color, primitive::Color);
    void update();
    void update(double);
    primitive::Color get_color() const { return color_; }
};

class ColorGenerator {
public:
    explicit ColorGenerator(std::initializer_list<primitive::Color> colors)
        : colors_{colors} {}

    ColorGenerator(ColorGenerator&&) = default;
    ColorGenerator& operator=(ColorGenerator&&) = default;
    ColorGenerator(ColorGenerator const&) = default;
    ColorGenerator& operator=(ColorGenerator const&) = default;
    ~ColorGenerator() = default;

    const primitive::Color& get() const
    {
        index_ = (index_ + 1) % colors_.size();
        return colors_.at(index_);
    }

private:
    std::vector<primitive::Color> colors_;
    mutable int index_{-1};
};

#endif // COLORSCHEMA_H
