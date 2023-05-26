struct EnumType {
  name: String,
  variants: Vec<String>,
}

enum TypeType {
  Builtin,
  Struct,
  Enum,
  Alias,
}

struct Type {
  name: String,
  type_type: TypeType,

}