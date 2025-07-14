#ifndef ACTION_H
#define ACTION_H

#include <functional>
#include <string>

struct Action {
	enum class Type {
		Rock,
		Paper,
		Scissors,
		Fold,
		Call,
		Raise,
		Empty,
		Invalid
	};

	Type type;

	Action() : type(Type::Empty) {}

	Action(Type t) : type(t) {}

	std::string toString() const {
		return std::to_string(static_cast<int>(type));
	}

	bool operator==(const Action &other) const {
		return type == other.type;
	}
};

namespace std {
template <>
struct hash<Action> {
	std::size_t operator()(const Action &a) const noexcept {
		// Hash the enum value by converting it to int
		return std::hash<int>()(static_cast<int>(a.type));
	}
};
} // namespace std

#endif // ACTION_H