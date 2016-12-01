{
    "targets": [
        {
            "target_name": "regex2",
            "sources": [ "regex2.cc", "functions.cc" ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ],
}
