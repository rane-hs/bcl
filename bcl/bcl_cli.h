#ifndef BCL_BCL_CLI_H
#define BCL_BCL_CLI_H


namespace bcl{
	
/// std::string -> System::String への変換
String^ ToSystemString(
	const std::string& i_src,
	System::Text::Encoding^ i_encoding = System::Text::Encoding::GetEncoding("shift_jis")
)
{
    return  gcnew System::String( i_src.data(), 0, i_src.size(), i_encoding );
}

/// System::String^ -> std::string の変換
const std::string ToStdString(
	String^ i_src,
	System::Text::Encoding^ i_encoding = System::Text::Encoding::GetEncoding("shift_jis") )
{
	std::string a_str;
	if( i_src != nullptr &&  i_src->Length > 0 ){
		array< Byte >^ a_array = System::Text::Encoding::Convert(
			System::Text::Encoding::Unicode, // 変換元エンコーディング
			i_encoding,          // 変換先エンコーディング
			System::Text::Encoding::Unicode->GetBytes( i_src ));
		pin_ptr<Byte> a_pin = &a_array[0];
		a_str.assign( reinterpret_cast<char*>(a_pin), a_array->Length );
	}
	return a_str;
}
	
}	//bcl::

#endif // BCL_BCL_CLI_H