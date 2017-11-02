# Teapot v2.2.0 configuration generated at 2017-10-19 11:13:29 +1300

required_version "2.0"

# Project Metadata

define_project "numerics" do |project|
	project.title = "Numerics"
	project.summary = 'A brief one line summary of the project.'
	
	project.description = <<-EOF
		Numerics description.
	EOF
	
	project.license = 'MIT License'
	
	project.add_author 'Samuel Williams', email: 'samuel.williams@oriontransfer.co.nz'
	# project.website = 'http://Numerics.com/'
	
	project.version = '0.1.0'
end

# Build Targets

define_target 'numerics-library' do |target|
	target.build do
		source_root = target.package.path + 'source'
		copy headers: source_root.glob('Numerics/**/*.{h,hpp}')
		build static_library: 'Numerics', source_files: source_root.glob('Numerics/**/*.cpp')
	end
	
	target.depends 'Build/Files'
	target.depends 'Build/Clang'
	
	target.depends :platform
	target.depends 'Language/C++14', private: true
	
	target.provides 'Library/Numerics' do
		append linkflags [
			->{install_prefix + 'lib/libNumerics.a'},
		]
	end
end

define_target 'numerics-test' do |target|
	target.build do |*arguments|
		test_root = target.package.path + 'test'
		
		run tests: 'Numerics', source_files: test_root.glob('Numerics/**/*.cpp'), arguments: arguments
	end
	
	target.depends 'Language/C++14', private: true
	
	target.depends 'Library/UnitTest'
	target.depends 'Library/Numerics'
	
	target.provides 'Test/Numerics'
end

define_target 'numerics-executable' do |target|
	target.build do
		source_root = target.package.path + 'source'
		
		build executable: 'Numerics', source_files: source_root.glob('Numerics.cpp')
	end
	
	target.depends 'Build/Files'
	target.depends 'Build/Clang'
	
	target.depends :platform
	target.depends 'Language/C++14', private: true
	
	target.depends 'Library/Numerics'
	target.provides 'Executable/Numerics'
end

define_target 'numerics-run' do |target|
	target.build do |*arguments|
		run executable: 'Numerics', arguments: arguments
	end
	
	target.depends 'Executable/Numerics'
	target.provides 'Run/Numerics'
end

# Configurations

define_configuration 'development' do |configuration|
	configuration[:source] = "https://github.com/kurocha"
	configuration.import "numerics"
	
	# Provides all the build related infrastructure:
	configuration.require 'platforms'
	
	# Provides unit testing infrastructure and generators:
	configuration.require 'unit-test'
	
	# Provides some useful C++ generators:
	configuration.require 'generate-cpp-class'
	
	configuration.require "generate-project"
	configuration.require "generate-travis"
end

define_configuration "numerics" do |configuration|
	configuration.public!
end
