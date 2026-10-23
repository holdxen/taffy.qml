fn main() {
    cxx_build::bridges(["src/lib.rs"])
        .std("c++17")
        .compile("taffy_cxx");
    println!("cargo:rerun-if-changed=src/lib.rs");
}
