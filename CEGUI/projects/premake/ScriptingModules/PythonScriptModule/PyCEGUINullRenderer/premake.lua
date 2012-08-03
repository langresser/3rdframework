--
-- Python extension module(s) premake script
--

cegui_dynamic("PyCEGUINullRenderer")
package.targetextension = "pyd"

-- Extension modules are always built as dlls (but linked statically)
if WANT_STATIC_BUILD then
	package.config.Debug_Static.kind = "dll"
	package.config.Release_Static.kind = "dll"

	define("BOOST_PYTHON_STATIC_LIB", "Debug_Static")
	define("BOOST_PYTHON_STATIC_LIB", "Release_Static")

	setup_static_core_libs()
	setup_static_xml_parser_libs(true)
	setup_static_image_codec_libs(true)
	setup_static_window_renderer_libs(true)
end

include(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings")

package.files =
{
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUINullRenderer/*.cpp"),
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUINullRenderer/*.hpp"),
}

if PYTHON_PATHS then
    add_sdk_paths(PYTHON_PATHS)
end

if BOOST_PYTHON_PATHS then
    add_sdk_paths(BOOST_PYTHON_PATHS)
end

dependency("CEGUIBase")
dependency("CEGUINullRenderer")

