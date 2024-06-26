#pragma once

#include "skeleton.h"

template <typename T>
class distance_surface :  public skeleton<T>
{
public:
	/// main templated superclass which we want to inherit stuff from
	typedef skeleton<T> base;
		using typename base::vec_type;
		using typename base::pnt_type;
		using base::points;
		using base::edges;
		using base::add_member_control;

protected:
	// reference radius of distance surface
	double r;
	// precomputed edge properties
	std::vector<vec_type> edge_vector, edge_vector_inv_length;
	// compute vector from closest point on skeleton edge i to point p
	vec_type get_edge_distance_vector(size_t i, const pnt_type &p) const;
	/// compute vector v from closest point on skeleton to point p and return value of implicit function
	double put_distance_vector(const pnt_type &p, vec_type& v) const;
	/// update helper variables for edge i
	virtual void update_edge_precomputations(size_t i);
public:
	/// construct distance surface
	distance_surface();
	/// overload to return the type name of this object
	std::string get_type_name() const { return "distance_surface"; }
	/// reflect members to expose them to serialization
	bool self_reflect(cgv::reflect::reflection_handler& rh);
	void append_edge_callback(size_t ei);
	void edge_changed_callback(size_t ei);
	void position_changed_callback(size_t pi);
	/// evaluate the implicit function itself, this must be overloaded
	T evaluate(const pnt_type& p) const;
	vec_type evaluate_gradient(const pnt_type& p) const;
protected:
	// allow derived classes to add the title of the gui
	bool gui_title_added;
public:
	void create_gui();
};
