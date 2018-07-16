#pragma once

#include <cgv/render/drawable.h>
#include <cgv/gui/provider.h>
#include <cgv/base/register.h>
#include <cgv/utils/convert_string.h>
#include "gl_implicit_surface_drawable.h"
#include <cgv/gui/text_editor.h>

using namespace cgv::render;
using namespace cgv::gui;
using namespace cgv::base;

struct abst_scene_factory
{
	std::string symbol;
	virtual void init_counter() = 0;
	virtual gl_implicit_surface_drawable::F* create_function() = 0;
};

extern void register_scene_factory(abst_scene_factory* _scene_factory);

template <typename T>
struct scene_factory : public abst_scene_factory
{
	static unsigned int& ref_counter() { 
		static unsigned int counter = 1;
		return counter;
	}
	static std::string& ref_base_name() {
		static std::string base_name;
		return base_name;
	}
	scene_factory(const std::string& _symbol, const std::string& base_name = "") {
		symbol = _symbol;
		ref_base_name() = base_name;
		if (base_name.empty()) {
			T dummy;
			if (dynamic_cast<base*>(&dummy))
				ref_base_name() = dynamic_cast<base*>(&dummy)->get_type_name();
		}
	}
	void init_counter() {
		ref_counter() = 1;
	}
	gl_implicit_surface_drawable::F* create_function() {
		gl_implicit_surface_drawable::F* f = new T;
		if (dynamic_cast<named*>(f))
			dynamic_cast<named*>(f)->set_name(ref_base_name()+"_"+
			cgv::utils::to_string(ref_counter()));
		++ref_counter();
		return f;
	};
};

/** use this registration struct to register a factory for your 
	 implementation of an implicit function.
*/
template <typename T>
struct scene_factory_registration
{
	scene_factory_registration(char _symbol, const std::string& _base_name = "") {
		register_scene_factory(new scene_factory<T>(std::string(1,_symbol), _base_name));
	}
	scene_factory_registration(const std::string& _symbol, const std::string& _base_name = "") {
		register_scene_factory(new scene_factory<T>(_symbol, _base_name));
	}
};

///
class scene : 
	public base, 
	public drawable, 
	public text_editor_callback_handler
{
private:
	bool disable_update;
	bool help_shown;
	void construct_editor();
protected:
	void show_help();
	/// store registered scene factories in a vector
	std::vector<abst_scene_factory*> factories;
	/// store the name of the current scene description file
	std::string file_name;
	/// store a pointer to the text editor
	cgv::gui::text_editor_ptr editor;
	// pass on property interface to text editor
	std::string get_property_declarations();
	bool set_void(const std::string& property, const std::string& value_type, const void* value_ptr);
	bool get_void(const std::string& property, const std::string& value_type, void* value_ptr);
	/// handle text changed events of text editor
	void text_changed(const char* _text, int length);
	/// called when new file has been read
	void after_read();
	/// called when text has been saved
	void after_save();
	/// if the editor is closed, it needs to be deleted
	void on_close_editor();
	/// rebuild gui
	void on_update_callback();
	/// called when nr_inserted characters have been inserted at text_pos
	void on_text_insertion(int text_pos, int nr_inserted);
	/// called when the nr_deleted characters in deleted_text have been deleted at text position text_pos
	void on_text_deletion(int text_pos, int nr_deleted, const char* deleted_text);
	/// update the style starting from text_pos. The text is unchanged after text_pos + min_nr_checked. Return the number of changed style characters. The default implementation does nothing, such that style A is kept for all characters and returns 0
	void update_style(int text_pos, int min_nr_checked);
public:
	/// type of the function describing the implicit surface
	typedef gl_implicit_surface_drawable::F F;
	/// pointer to implicit surface drawable
	gl_implicit_surface_drawable_ptr impl_draw_ptr;
	/// pointer to current function
	base_ptr func_base_ptr;
	/// current scene description
	std::string description;

	std::string get_changed_values(F* fp, F* fp_ref) const;
	void reconstruct_description();
	std::string reconstruct_description_recursive(unsigned int& i, F* func_ptr, group* g);
	/// check if factory's symbol[s] match location i in description
	bool symbol_matches_description(unsigned int i, abst_scene_factory* factory, unsigned int& offset) const;
	/// recursive part of the scene description parsing
	F* parse_description_recursive(unsigned int& i, group* g);
	/// parse a scene description and construct a function pointer
	void parse_description();
	/// callback for functions that update the scene based on gui interaction
	void update_scene();
	/// callback for functions that update the scene description without the implicit function
	void update_description();
	/// registration of scene factories;
	void register_factory(abst_scene_factory* _scene_factory);
	/// construct scene from a description string
	scene(const std::string& _description = "S");
	/// called to unregister derived guis and drawables
	void unregister();
	/// overload to return the type name of this object
	std::string get_type_name() const;
};

/** interface for function implementations that change the
    scene based on gui interaction. */
class scene_updater
{
private:
	scene* s;
public:
	/// default constructor
	scene_updater();
	/// access to scene pointer
	scene* get_scene() const;
	/// set the scene pointer. Can be overloaded to pass pointer on to children of a group.
	virtual void set_scene(scene* _s);
	/// to be called if scene has changed due to gui interaction
	void update_scene();
	/// callback for functions that update the scene description without the implicit function
	void update_description();
};

/// ref counted pointer to a scene
typedef cgv::data::ref_ptr<scene> scene_ptr;
