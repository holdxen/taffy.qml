use crate::utils::ffi::FlexDirection;

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


impl From<FlexDirection> for taffy::FlexDirection {
    fn from(value: FlexDirection) -> Self {
        match value {
            FlexDirection::FlexDirectionColumn => taffy::FlexDirection::Column,
            FlexDirection::FlexDirectionColumnReverse => taffy::FlexDirection::ColumnReverse,
            FlexDirection::FlexDirectionRow => taffy::FlexDirection::Row,
            FlexDirection::FlexDirectionRowReverse => taffy::FlexDirection::RowReverse,
            _ => Default::default()
        }
    }
}


impl From<taffy::FlexDirection> for FlexDirection {
    fn from(value: taffy::FlexDirection) -> Self {
        match value {
            taffy::FlexDirection::Row => Self::FlexDirectionRow,
            taffy::FlexDirection::Column => Self::FlexDirectionColumn,
            taffy::FlexDirection::RowReverse => Self::FlexDirectionRowReverse,
            taffy::FlexDirection::ColumnReverse => Self::FlexDirectionColumnReverse
        }
    }
}


#[derive(Default)]
pub struct TaffyTypeRust;