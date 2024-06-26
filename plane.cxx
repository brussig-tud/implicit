#include "implicit_primitive.h"

template <typename T>
struct plane : public implicit_primitive<T>
{
	/// main templated superclass which we want to inherit stuff from
	typedef implicit_primitive<T> base;
		using typename base::vec_type;
		using typename base::pnt_type;
		using typename base::clr_type;
		using base::gui_color;
		using base::evaluate_gradient;
		using base::add_gui;
		using base::add_member_control;

	vec_type normal;
	double   distance;

	plane() : normal(0, 0, 1), distance(0) {
		gui_color = 0xFF8888;
	}

	bool self_reflect(cgv::reflect::reflection_handler& rh)
	{
		return
			rh.reflect_member("d", distance) &&
			rh.reflect_member("nx", normal(0)) &&
			rh.reflect_member("ny", normal(1)) &&
			rh.reflect_member("nz", normal(2)) &&
			base::self_reflect(rh);
	}

	void on_set(void* member_ptr)
	{
		base::update_member(member_ptr);
		base::update_scene();
	}
	std::string get_type_name() const { return "plane"; }
	/// evaluate the implicit function itself, this must be overloaded
	T evaluate(const pnt_type& p) const {
		return dot(p, normal) - distance;
	}
	vec_type evaluate_gradient(const pnt_type& p) const { return normal; }
	void create_gui()
	{
		add_gui("normal", normal, "direction", "options='min=-1;max=1;ticks=true'");
		add_member_control(this, "distance", distance, "value_slider", "min=-2;max=2;ticks=true");
	}
};

scene_factory_registration<plane<double> >sfr_plane("plane;P");
