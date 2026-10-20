

#[cxx_qt::bridge]
pub mod ffi {

    #[qenum(TaffyType)]
    enum FlexDirection {
        FlexDirectionRow,
        FlexDirectionColumn,
        FlexDirectionRowReverse,
        FlexDirectionColumnReverse,
    }

    extern "RustQt" {
        #[qobject]
        #[qml_element]
        type TaffyType = super::TaffyTypeRust;
    }
}



#[derive(Default)]
pub struct TaffyTypeRust;