<?php
include 'Config.php';

///-------------------------------------------------------------------------------------------------
/// @brief      Wrapper class for the web encoder
///-------------------------------------------------------------------------------------------------
class Crypto
{
    ///-------------------------------------------------------------------------------------------------
    /// @brief      Encrypts a strign with AES256GCM
    ///
    /// @param[in]  $input       The input
    /// @param[in]  $key         The key
    /// @param[in]  $iv          The iv
    /// @param[in]  $url_encode  The url encode
    ///
    /// @return     The AES cipher if success, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static public function aes_encrypt($input, $key, $iv, $url_encode = false) {
        $command = sprintf("cd %s && ./%s -e \"aes\" -i %s --key %s --iv %s",
                           escapeshellarg(Config::Encoder_Path),
                           Config::Encoder_Name,
                           escapeshellarg($input),
                           escapeshellarg($key),
                           escapeshellarg($iv)
        );
        if($url_encode) {
            $command = sprintf("$s -u", $command);
        }
        return exec($command);
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Decrypts a AES 256 GCM cipher.
    ///
    /// @param[in]  $cipher  The cipher
    /// @param[in]  $key     The key
    /// @param[in]  $iv      The iv
    ///
    /// @return     The plain string if success, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static public function aes_decrypt($cipher, $key, $iv) {
        $command = sprintf("cd %s && ./%s -d \"aes\" -i %s --key %s --iv %s",
                           escapeshellarg(Config::Encoder_Path),
                           Config::Encoder_Name,
                           escapeshellarg($cipher),
                           escapeshellarg($key),
                           escapeshellarg($iv)
        );
        return exec($command);
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Encrypts a string with RSA.
    ///
    /// @param[in]  $input       The input
    /// @param[in]  $public_key  The public key
    ///
    /// @return     The RSA cipher if success, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static public function rsa_encrypt($input, $public_key) {
        $command = sprintf("cd %s && ./%s -e \"rsa\" -i %s --public %s",
                           escapeshellarg(Config::Encoder_Path),
                           Config::Encoder_Name,
                           escapeshellarg($input),
                           escapeshellarg($public_key)
        );
        return exec($command);
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Decrypts a RSA cipher.
    ///
    /// @param[in]  $cipher       The cipher
    /// @param[in]  $private_key  The private key
    ///
    /// @return     The plain text if success, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static public function rsa_decrypt($cipher, $private_key) {
        $command = sprintf("cd %s && ./%s -d \"rsa\" -i %s --private %s",
                           escapeshellarg(Config::Encoder_Path),
                           Config::Encoder_Name,
                           escapeshellarg($cipher),
                           escapeshellarg($private_key)
        );
        return exec($command);
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Generates a new AES 256Bit GCM key pair.
    ///
    /// @return     An array with ["key"|"iv"] index if success, null otherwise.
    ///-------------------------------------------------------------------------------------------------
    static public function generate_aes_key_pair() {
        $command = sprintf("cd %s && ./%s -g",
                           escapeshellarg(Config::Encoder_Path),
                           Config::Encoder_Name
        );
        exec($command);

        $keypair_file_name = sprintf("%s/keypair.txt", Config::Encoder_Path);
        
        $file = file_get_contents($keypair_file_name);
        if(empty($file)) {
            /// obv. can't find the keypair.txt file
            return null;
        }

        $kp = array();
        $has_key = false;
        foreach(explode("\n", $file) as $line) {
            if(empty($line)) {
                continue;
            }
            if(strpos($line, '///') !== false) {
                continue;
            }

            if(!$has_key) {
                $kp["key"] = $line;
                $has_key = true;
            }
            else {
                $kp["iv"] = $line;
            }
        }

        exec(sprintf("rm %s", $keypair_file_name));

        return $kp;
    }
}
