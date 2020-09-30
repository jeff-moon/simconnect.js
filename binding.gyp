{
    "targets": [{
        "target_name": "SimConnectJs",
        "cflags!": ["-fno-exceptions"],
        "cflags_cc!": ["-fno-exceptions"],
        "sources": [
            "src/index.cpp",
            "src/simconnectjs.cpp",
            "src/worker/call_dispatch_worker.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
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
