#pragma once

#include <cgv_gl/gl/gl_implicit_surface_drawable_base.h>
#include <cgv/base/base.h>
#include <cgv/gui/provider.h>
#include <cgv/gui/event_handler.h>

/** drawable that visualizes implicit surfaces by contouring them with marching cubes or
    dual contouring. */
class gl_implicit_surface_drawable : 
	public cgv::base::node, 
	public cgv::gui::provider,
	public cgv::gui::event_handler,
	public cgv::render::gl::gl_implicit_surface_drawable_base
{
protected:
	double map_to_zero_value;
	double map_to_one_value;
	cgv::render::view* view_ptr;
	void toggle_range();
	void adjust_range();
	void export_volume();
	void extract_mesh();

	void save_interactive();
	void resolution_change();
	void surface_extraction();
public:
	/// standard constructor does not initialize the function pointer such that nothing is drawn
	gl_implicit_surface_drawable(const std::string& name);
	void on_set(void* member_ptr);
	bool self_reflect(cgv::reflect::reflection_handler& rh);
	std::string get_type_name() const;
	void stream_help(std::ostream& os);
	bool handle(cgv::gui::event& e);
	bool init(cgv::render::context& ctx);
	void create_gui();
};

typedef cgv::data::ref_ptr<gl_implicit_surface_drawable> gl_implicit_surface_drawable_ptr;