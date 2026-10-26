fn main() {
    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-env-changed=TAFFY_CXX_FROM_CMAKE");

    // When built through CMake, corrosion_add_cxxbridge compiles the glue.
    // Skip here so the same symbols are not emitted twice.
    if std::env::var_os("TAFFY_CXX_FROM_CMAKE").is_some() {
        return;
    }

    cxx_build::bridges(["src/lib.rs"])
        .std("c++17")
        .compile("taffy_cxx");
}
