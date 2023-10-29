#pragma once
#include "Core/filepath.h"
#include "doctest/doctest.h"

TEST_SUITE("FilePath") {
	TEST_CASE("Working directory") {
		fm::FilePath path(fm::LOCATION::WORKING_DIRECTORY, "");
		CHECK(path.GetPathString() == fs::current_path());
	}
	TEST_CASE("Extension") {
		fm::FilePath path(fm::LOCATION::WORKING_DIRECTORY, "");
		path.ToSub("file.test");
		CHECK(path.GetExtension() == ".test");
	}
	TEST_CASE("Extension_2") {
		fm::FilePath path(fm::LOCATION::WORKING_DIRECTORY, "");
		path.ToSub("file.test.all");
		CHECK(path.GetExtension() == ".all");
	}
	TEST_CASE("Name") {
		fm::FilePath path(fm::LOCATION::WORKING_DIRECTORY, "");
		path.ToSub("file.test.all");
		CHECK(path.GetName(true) == "file.test");
		CHECK(path.GetName(false) == "file.test.all");
	}

	TEST_CASE("Parent") {
		fm::FilePath path(fm::LOCATION::WORKING_DIRECTORY, "");
		path.ToSub("file");
		path.ToSub("file_2");
		CHECK(path.GetParent().GetName(true) == "file");
	}
}

