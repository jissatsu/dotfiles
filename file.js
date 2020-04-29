const fs = require('fs');

exports.listing = {};

// check if entry is directory
exports.is_dir = ( dirname ) => {
    fs.lstat( dirname, function(error, stats) {
        if ( error ) {
            throw error;
        }
        if ( stats.isDirectory() ) {
            return true;
        }
    });
    return false;
}

// check if the file is hidden
exports.is_hidden = ( filepath ) => {
    if ( typeof filepath == "string" && fs.exists( filepath ) ) {
        if ( filepath.substring(0, 1) == "." ) {
            return true;
        }
    }
    return false;
}

// get file extension
exports.extension = ( filepath ) => {
    if ( typeof filepath != "string" || filepath.indexOf('.') < 0 ) {
        return "";
    }
    let r = filepath.split('.');
    return r[r.length - 1];
};

// list directory contents
exports.list_dir = ( dir ) => {
    (function(object){
        fs.readdir( dir, function( error, items ) {
            if ( error ) {
                throw error;
            } else {
                for ( var i in items ) {
                    object.listing[i] = items[i];
                }
            }
        });
    })(this);
    console.log( this.listing );
};

// move file somewhere
exports.move = ( filepath, dirname ) => {
    fs.rename(filepath, dirname, function(error) {
        if ( error ) {
            throw error;
        }
    });
    return true;
}

// delete file
exports.delete = ( filepath ) => {
    fs.unlink(filepath, function(error) {
        if ( error ) {
            throw error;
        };
    });
    return true;
}