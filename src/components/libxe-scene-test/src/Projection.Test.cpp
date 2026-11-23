
#include <catch2/catch_test_macros.hpp>

#include <xe/math/Common.h>
#include <xe/scene/Projection.h>

TEST_CASE("ProjectionTest.ProjectFunction", "[scene][projection]") {
    // When considering a default transformation pipeline, with a viewport of (0, 0) - (640, 480)
    const xe::Matrix4 projViewModel = xe::mat4Identity();
    const xe::Viewport viewport = {{0, 0}, {640, 480}};

    // SECTION("Points placed into the XY plane at Z = 0 should map directly inside the Viewport") {
    REQUIRE(xe::project({0.0f, 0.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(320.0f, 240.0f, 0.0f, 1.0f));
    REQUIRE(xe::project({-1.0f, 0.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(0.0f, 240.0f, 0.0f, 1.0f));
    REQUIRE(xe::project({1.0f, 0.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(640.0f, 240.0f, 0.0f, 1.0f));
    REQUIRE(xe::project({0.0f, 1.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(320.0f, 480.0f, 0.0f, 1.0f));
    REQUIRE(xe::project({0.0f, -1.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(320.0f, 0.0f, 0.0f, 1.0f));

    // SECTION("and also At Z=0.5") {
    REQUIRE(xe::project({0.0f, 0.0f, 0.5f, 1.0f}, projViewModel, viewport) == xe::Vector4(320.0f, 240.0f, 0.5f, 1.0f));
    REQUIRE(xe::project({-1.0f, 0.0f, 0.5f, 1.0f}, projViewModel, viewport) == xe::Vector4(0.0f, 240.0f, 0.5f, 1.0f));
    REQUIRE(xe::project({1.0f, 0.0f, 0.5f, 1.0f}, projViewModel, viewport) == xe::Vector4(640.0f, 240.0f, 0.5f, 1.0f));
    REQUIRE(xe::project({0.0f, 1.0f, 0.5f, 1.0f}, projViewModel, viewport) == xe::Vector4(320.0f, 480.0f, 0.5f, 1.0f));
    REQUIRE(xe::project({0.0f, -1.0f, 0.5f, 1.0f}, projViewModel, viewport) == xe::Vector4(320.0f, 0.0f, 0.5f, 1.0f));

    // SECTION("Points placed outside the [-1.0, 1.0]^2 range map outside the viewport") {
    REQUIRE(xe::project({-2.0f, 0.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(-320.0f, 240.0f, 0.0f, 1.0f));
    REQUIRE(xe::project({-2.0f, 2.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(-320.0f, 720.0f, 0.0f, 1.0f));

    // SECTION("Vectors (with W = 0) should keep pointing in the same direction") {
    const auto projected = xe::project({1.0f, 1.0f, 0.0f, 0.0f}, projViewModel, viewport);
    REQUIRE(projected == xe::Vector4(640.0f, 480.0f, 0.0f, 0.0f));
}

TEST_CASE("ProjectionTest.ProjectFunctionPerpective", "[scene][projection]") {
    const xe::Matrix4 proj = xe::mat4Perspective(xe::radians(90.0f), 640.0f / 480.0f, 0.1f, 100.0f);
    const xe::Matrix4 view = xe::mat4LookAtRH({0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    const xe::Matrix4 projViewModel = proj * view;
    const xe::Viewport viewport = {{0, 0}, {640, 480}};

    REQUIRE(xe::project({0.0f, 0.0f, 0.0f, 1.0f}, projViewModel, viewport).x == 320.0f);
    REQUIRE(xe::project({0.0f, 0.0f, 0.0f, 1.0f}, projViewModel, viewport).y == 240.0f);
}

TEST_CASE("ProjectionTest.UnprojectFunction", "[scene][projection]") {
    // SECTION("When considering a default transformation pipeline, with a viewport of (0, 0) - (640, 480)") {
    const xe::Matrix4 projViewModel = xe::mat4Identity();
    const xe::Viewport viewport = {{0, 0}, {640, 480}};

    // SECTION("Points placed inside the screen are in the [-1, 1]^2 domain") {
    REQUIRE(xe::unproject({320.0f, 240.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    REQUIRE(xe::unproject({0.0f, 240.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(-1.0f, 0.0f, 0.0f, 1.0f));
    REQUIRE(xe::unproject({640.0f, 240.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(1.0f, 0.0f, 0.0f, 1.0f));
    REQUIRE(xe::unproject({320.0f, 480.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
    REQUIRE(xe::unproject({320.0f, 0.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(0.0f, -1.0f, 0.0f, 1.0f));

    // SECTION("Points placed outside the screen are out of the [-1.0, 1.0]^2") {
    REQUIRE(xe::unproject({-320.0f, 240.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(-2.0f, 0.0f, 0.0f, 1.0f));
    REQUIRE(xe::unproject({-320.0f, 720.0f, 0.0f, 1.0f}, projViewModel, viewport) == xe::Vector4(-2.0f, 2.0f, 0.0f, 1.0f));
}
