set_xmakever("2.9.3")

set_project("OpenGLTest")

add_rules("mode.debug", "mode.release")

if is_mode("release") then
  set_optimize("fastest")
else
  add_defines("OGLTEST_DEBUG")
end

add_requires("glad", "glfw", "glm", "stb", "assimp")

set_encodings("utf-8")
set_exceptions("cxx")
set_languages("cxx20")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")
set_warnings("allextra")

option("use_pch", {description = "Use the precompiled header to speed up compilation speeds.", default = true})

rule("cp-resources")
  after_build(function (target)
    os.cp("Resources", "bin/$(plat)_$(arch)_$(mode)")
  end)

includes("xmake/**.lua")

target("OpenGLTest")
    set_kind("binary")

    add_rules("cp-resources")

    add_files("Source/OpenGLTest/**.cpp")
    for _, ext in ipairs({".hpp", ".inl"}) do
      add_headerfiles("Include/**" .. ext)
    end
    
    add_includedirs("Include/", {public = true})
    
    for _, ext in ipairs({".vert", ".frag", ".png", ".mtl", ".obj", ".jpg"}) do
      add_extrafiles("Resources/**" .. ext)
    end

    if has_config("use_pch") then
      set_pcxxheader("Include/OpenGLTest/pch.hpp")
    end
      
    add_packages("glad", "glfw", "glm", "stb", "assimp")
