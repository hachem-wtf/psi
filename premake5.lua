output_dir = "%{cfg.buildcfg}-%{cfg.system}"

function setup_target()
	targetdir ("bin/" .. output_dir)
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")
	staticruntime "On"
end

function setup_c_target()
	setup_target()

	filter "configurations:debug"
		runtime "Debug"
		symbols "On"
	filter { "configurations:release", "configurations:dist" }
		runtime "Release"
		optimize "Speed"
	filter "configurations:dist"
		symbols "Off"
	filter {}
end

newaction {
	trigger = "clean",
	description = "Remove build output and generated project files",

	execute = function()
		os.rmdir("bin")
		os.rmdir("bin-int")
		os.remove("Makefile")

		for _, file in ipairs(os.matchfiles("*.make")) do
			os.remove(file)
		end

		print("Cleaned build output and generated project files")
	end
}

workspace "psi"
	architecture "x64"
	startproject "tester"
	multiprocessorcompile "On"

	configurations {
		"debug",
		"release",
		"dist",
	}

	filter "system:windows"
		defines "PSI_WINDOWS"
	filter "system:linux"
		defines "PSI_LINUX"
	filter "system:macosx"
		architecture "ARM64"
		defines "PSI_MACOS"
	filter {}

project "psi"
	kind "StaticLib"
	language "C"
	cdialect "C17"

	setup_c_target()

	files {
		"src/**.h",
		"src/**.c",
	}

	includedirs "src"

	filter { "system:macosx or system:linux" }
		buildoptions {
			"-Wall",
			"-Wextra",
			"-Werror",
			"-pedantic",
		}
	filter "system:windows"
		systemversion "latest"
	filter {}

	filter "configurations:debug"
		defines "PSI_DEBUG"
	filter "configurations:release"
		defines "PSI_RELEASE"
	filter "configurations:dist"
		defines "PSI_DIST"
	filter {}

project "tester"
	kind "ConsoleApp"
	language "C"
	cdialect "C17"

	setup_c_target()

	files {
		"tester/**.h",
		"tester/**.c",
	}

	includedirs {
		"src",
		"tester",
	}

	links "psi"

	filter { "system:macosx or system:linux" }
		links "m"

		buildoptions {
			"-Wall",
			"-Wextra",
			"-Werror",
			"-pedantic",
		}
	filter "system:windows"
		systemversion "latest"
	filter {}

	filter "configurations:debug"
		defines "PSI_DEBUG"
	filter "configurations:release"
		defines "PSI_RELEASE"
	filter "configurations:dist"
		defines "PSI_DIST"
	filter {}
