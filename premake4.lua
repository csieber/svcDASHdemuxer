solution "DASH"
  configurations { "Debug", "Release" }

  targetdir "bin/"

  if _ACTION == "clean" then
    os.rmdir("bin/")
    os.rmdir("build/")
  end

  objdir "build/obj"
  location "build/"
  includedirs { "." }

--##################
--# Project: svcDASHdemuxer
--##################

project "svcDASHdemuxer"
  language "C++"
  kind "ConsoleApp"
  files { "src/**.cpp" }

  links { "boost_system",
          "boost_program_options" }

configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }
    targetname ( "svcDASHdemuxer_d" )

  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" }
    targetname ( "svcDASHdemuxer" )

