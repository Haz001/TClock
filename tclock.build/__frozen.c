// This provides the frozen (compiled bytecode) files that are included if
// any.
#include <Python.h>

#include "nuitka/constants_blob.h"

// Blob from which modules are unstreamed.
#define stream_data constant_bin

// These modules should be loaded as bytecode. They may e.g. have to be loadable
// during "Py_Initialize" already, or for irrelevance, they are only included
// in this un-optimized form. These are not compiled by Nuitka, and therefore
// are not accelerated at all, merely bundled with the binary or module, so
// that CPython library can start out finding them.

struct frozen_desc {
    char const *name;
    ssize_t start;
    int size;
};

void copyFrozenModulesTo( struct _frozen *destination )
{
    struct frozen_desc frozen_modules[] = {
        { "base64", 6368626, 11253 },
        { "codecs", 6379879, 36628 },
        { "copy_reg", 6416507, 5137 },
        { "encodings", 6421644, -4362 },
        { "encodings.aliases", 6426006, 8760 },
        { "encodings.ascii", 6434766, 2253 },
        { "encodings.base64_codec", 6437019, 3829 },
        { "encodings.big5", 6440848, 1748 },
        { "encodings.big5hkscs", 6442596, 1788 },
        { "encodings.bz2_codec", 6444384, 4721 },
        { "encodings.charmap", 6449105, 3465 },
        { "encodings.cp037", 6452570, 2829 },
        { "encodings.cp1006", 6455399, 2915 },
        { "encodings.cp1026", 6458314, 2843 },
        { "encodings.cp1140", 6461157, 2829 },
        { "encodings.cp1250", 6463986, 2866 },
        { "encodings.cp1251", 6466852, 2863 },
        { "encodings.cp1252", 6469715, 2866 },
        { "encodings.cp1253", 6472581, 2879 },
        { "encodings.cp1254", 6475460, 2868 },
        { "encodings.cp1255", 6478328, 2887 },
        { "encodings.cp1256", 6481215, 2865 },
        { "encodings.cp1257", 6484080, 2873 },
        { "encodings.cp1258", 6486953, 2871 },
        { "encodings.cp424", 6489824, 2859 },
        { "encodings.cp437", 6492683, 8064 },
        { "encodings.cp500", 6500747, 2829 },
        { "encodings.cp720", 6503576, 2926 },
        { "encodings.cp737", 6506502, 8292 },
        { "encodings.cp775", 6514794, 8078 },
        { "encodings.cp850", 6522872, 7811 },
        { "encodings.cp852", 6530683, 8080 },
        { "encodings.cp855", 6538763, 8261 },
        { "encodings.cp856", 6547024, 2891 },
        { "encodings.cp857", 6549915, 7801 },
        { "encodings.cp858", 6557716, 7781 },
        { "encodings.cp860", 6565497, 8047 },
        { "encodings.cp861", 6573544, 8058 },
        { "encodings.cp862", 6581602, 8193 },
        { "encodings.cp863", 6589795, 8058 },
        { "encodings.cp864", 6597853, 8189 },
        { "encodings.cp865", 6606042, 8058 },
        { "encodings.cp866", 6614100, 8293 },
        { "encodings.cp869", 6622393, 8105 },
        { "encodings.cp874", 6630498, 2957 },
        { "encodings.cp875", 6633455, 2826 },
        { "encodings.cp932", 6636281, 1756 },
        { "encodings.cp949", 6638037, 1756 },
        { "encodings.cp950", 6639793, 1756 },
        { "encodings.euc_jis_2004", 6641549, 1812 },
        { "encodings.euc_jisx0213", 6643361, 1812 },
        { "encodings.euc_jp", 6645173, 1764 },
        { "encodings.euc_kr", 6646937, 1764 },
        { "encodings.gb18030", 6648701, 1772 },
        { "encodings.gb2312", 6650473, 1764 },
        { "encodings.gbk", 6652237, 1740 },
        { "encodings.hex_codec", 6653977, 3781 },
        { "encodings.hp_roman8", 6657758, 4112 },
        { "encodings.hz", 6661870, 1732 },
        { "encodings.idna", 6663602, 6368 },
        { "encodings.iso2022_jp", 6669970, 1801 },
        { "encodings.iso2022_jp_1", 6671771, 1817 },
        { "encodings.iso2022_jp_2", 6673588, 1817 },
        { "encodings.iso2022_jp_2004", 6675405, 1841 },
        { "encodings.iso2022_jp_3", 6677246, 1817 },
        { "encodings.iso2022_jp_ext", 6679063, 1833 },
        { "encodings.iso2022_kr", 6680896, 1801 },
        { "encodings.iso8859_1", 6682697, 2868 },
        { "encodings.iso8859_10", 6685565, 2883 },
        { "encodings.iso8859_11", 6688448, 2977 },
        { "encodings.iso8859_13", 6691425, 2886 },
        { "encodings.iso8859_14", 6694311, 2904 },
        { "encodings.iso8859_15", 6697215, 2883 },
        { "encodings.iso8859_16", 6700098, 2885 },
        { "encodings.iso8859_2", 6702983, 2868 },
        { "encodings.iso8859_3", 6705851, 2875 },
        { "encodings.iso8859_4", 6708726, 2868 },
        { "encodings.iso8859_5", 6711594, 2869 },
        { "encodings.iso8859_6", 6714463, 2913 },
        { "encodings.iso8859_7", 6717376, 2876 },
        { "encodings.iso8859_8", 6720252, 2907 },
        { "encodings.iso8859_9", 6723159, 2868 },
        { "encodings.johab", 6726027, 1756 },
        { "encodings.koi8_r", 6727783, 2890 },
        { "encodings.koi8_u", 6730673, 2876 },
        { "encodings.latin_1", 6733549, 2283 },
        { "encodings.mac_arabic", 6735832, 8014 },
        { "encodings.mac_centeuro", 6743846, 2937 },
        { "encodings.mac_croatian", 6746783, 2945 },
        { "encodings.mac_cyrillic", 6749728, 2935 },
        { "encodings.mac_farsi", 6752663, 2849 },
        { "encodings.mac_greek", 6755512, 2889 },
        { "encodings.mac_iceland", 6758401, 2928 },
        { "encodings.mac_latin2", 6761329, 4907 },
        { "encodings.mac_roman", 6766236, 2906 },
        { "encodings.mac_romanian", 6769142, 2946 },
        { "encodings.mac_turkish", 6772088, 2929 },
        { "encodings.palmos", 6775017, 3066 },
        { "encodings.ptcp154", 6778083, 4890 },
        { "encodings.punycode", 6782973, 7942 },
        { "encodings.quopri_codec", 6790915, 3647 },
        { "encodings.raw_unicode_escape", 6794562, 2202 },
        { "encodings.rot_13", 6796764, 3656 },
        { "encodings.shift_jis", 6800420, 1788 },
        { "encodings.shift_jis_2004", 6802208, 1828 },
        { "encodings.shift_jisx0213", 6804036, 1828 },
        { "encodings.string_escape", 6805864, 2061 },
        { "encodings.tis_620", 6807925, 2938 },
        { "encodings.undefined", 6810863, 2589 },
        { "encodings.unicode_escape", 6813452, 2150 },
        { "encodings.unicode_internal", 6815602, 2176 },
        { "encodings.utf_16", 6817778, 5160 },
        { "encodings.utf_16_be", 6822938, 1990 },
        { "encodings.utf_16_le", 6824928, 1990 },
        { "encodings.utf_32", 6826918, 5724 },
        { "encodings.utf_32_be", 6832642, 1883 },
        { "encodings.utf_32_le", 6834525, 1883 },
        { "encodings.utf_7", 6836408, 1883 },
        { "encodings.utf_8", 6838291, 1942 },
        { "encodings.utf_8_sig", 6840233, 4977 },
        { "encodings.uu_codec", 6845210, 4909 },
        { "encodings.zlib_codec", 6850119, 4641 },
        { "functools", 6854760, 6561 },
        { "locale", 6861321, 56524 },
        { "quopri", 6917845, 6544 },
        { "re", 6924389, 13363 },
        { "sre_compile", 6937752, 12522 },
        { "sre_constants", 6950274, 6177 },
        { "sre_parse", 6956451, 21076 },
        { "string", 6977527, 20349 },
        { "stringprep", 6997876, 14439 },
        { "struct", 7012315, 229 },
        { "types", 7012544, 2703 },
        { NULL, 0, 0 }
    };

    struct frozen_desc *current = frozen_modules;

    for(;;)
    {
        destination->name = (char *)current->name;
        destination->code = (unsigned char *)&constant_bin[ current->start ];
        destination->size = current->size;

        if (destination->name == NULL) break;

        current += 1;
        destination += 1;
    };
}
