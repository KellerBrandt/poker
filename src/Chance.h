#ifndef CHANCE_H
#define CHANCE_H

struct Chance {
    int value;
    Chance() : value(-1) {}
    Chance(int v) : value(v) {}

    bool operator==(const Chance &other) const {
		return value == other.value;
	}
};

namespace std {
    template <>
    struct hash<Chance> {
        std::size_t operator()(const Chance &c) const noexcept {
            // Hash the enum value by converting it to int
            return std::hash<int>()(static_cast<int>(c.value));
        }
    };
    } // namespace std

#endif // CHANCE_H 