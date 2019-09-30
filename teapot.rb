# Teapot v2.2.0 configuration generated at 2017-10-19 11:13:29 +1300

required_version "3.0"

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
	target.depends 'Language/C++14', private: true
	
	target.provides 'Library/Numerics' do
		source_root = target.package.path + 'source'
		
		library_path = build static_library: 'Numerics', source_files: source_root.glob('Numerics/**/*.cpp')
		
		append linkflags library_path
		append header_search_paths source_root
	end
end

define_target 'numerics-test' do |target|
	target.depends 'Language/C++14'
	
	target.depends 'Library/UnitTest'
	target.depends 'Library/Numerics'
	
	target.provides 'Test/Numerics' do |*arguments|
		test_root = target.package.path + 'test'
		
		run source_files: test_root.glob('Numerics/**/*.cpp'), arguments: arguments
	end
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
