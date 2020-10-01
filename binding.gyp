{
    "targets": [{
        "target_name": "SimConnectJs",
        "cflags!": ["-fno-exceptions"],
        "cflags_cc!": ["-fno-exceptions"],
        "sources": [
            "src/addon.cc",
            "src/addon_node.cc",
            "src/object/napi_object_helper.cc"
        ],
        'include_dirs': [
            "./ext"
        ],
        "dll_files": [
            "SimConnect.dll"
        ],
        'libraries': [
            "<(module_root_dir)/lib/SimConnect.lib"
        ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS']
    }]
}
