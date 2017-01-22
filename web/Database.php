<?php
include 'Crypto.php';

///-------------------------------------------------------------------------------------------------
/// @brief      Wrapper class for the MySQL database/table
///-------------------------------------------------------------------------------------------------
class Database
{
    private $m_Database = null;

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Builds an unique hwardware id hash.
    ///
    /// @param[in]  $username             The username
    /// @param[in]  $cpu                  The cpu
    /// @param[in]  $computer_name        The computer name
    /// @param[in]  $physical_hdd_serial  The physical hdd serial
    ///
    /// @return     The hwid hash.
    ///-------------------------------------------------------------------------------------------------
    static public function build_hwid_hash($username, $cpu, $computer_name, $physical_hdd_serial) {
        if(!isset($username)      ||
           !isset($cpu)           ||
           !isset($computer_name) ||
           !isset($physical_hdd_serial)) {
            return false;
        }
  
        return sha1(sprintf("%s-%s-%s-%s",
                            hash("sha256", $username),
                            hash("sha256", $physical_hdd_serial),
                            hash("sha256", $computer_name),
                            hash("sha256", $cpu
        )));
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Connect to the database.
    ///
    /// @return     True if success, false otherwise.
    ///-------------------------------------------------------------------------------------------------
    public function connect() {
        $this->m_Database = new mysqli(Config::MySQL_Hostname,
                                       Config::MySQL_Username,
                                       Config::MySQL_Password,
                                       Config::MySQL_Database
        );
        if(mysqli_connect_errno() != 0) {
            $this->m_Database->close();
            unset($this->m_Database);
            return false;
        }
        return true;
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Disconnect from the database.
    ///-------------------------------------------------------------------------------------------------
    public function disconnect() {
        if(isset($this->m_Database)) {
            $this->m_Database->close();
            unset($this->m_Database);
        }
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Register a new user.
    ///
    /// @param[in]  $username  The username
    ///
    /// @return     True if sucess, false otherwise.
    ///-------------------------------------------------------------------------------------------------
    public function register_user($username) {
        if(!$this->m_Database ||
           !isset($username)) {
            return false;
        }
        $user_data = $this->query_user_data($username);
        if($user_data != null) {
            return false;
        }

        $sql_state = $this->m_Database->prepare("INSERT INTO `userlist` VALUES (?,\"\",?,?,\"\",\"\",\"\",\"\")");
        if(!isset($sql_state)) {
            return false;
        }
        if(!$sql_state->bind_param('sii', $username, 0, 1)) {
            $sql_state->close();
            return false;
        }
        
        $updated = $sql_state->execute();
        $sql_state->close();
        return $updated;
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Queries an user data.
    ///
    /// @param[in]  $username  The username
    ///
    /// @return     An array with the userdata if success, null otherwise.
    ///-------------------------------------------------------------------------------------------------
    public function query_user_data($username) {
        if(!$this->m_Database) {
            return null;
        }

        $sql_state = $this->m_Database->prepare("SELECT `username`, `password`, `banned`, `fail_counter`, `cpu`, `computer_name`, `physical_hdd_serial`, `hwid_hash` FROM userlist WHERE `username` = ? LIMIT 1");
        if(!isset($sql_state)) {
            return null;
        }
        if(!$sql_state->bind_param('s', $username)) {
            $sql_state->close();
            return null;
        }
        if(!$sql_state->execute()) {
            $sql_state->close();
            return null;
        }
        if(!$sql_state->store_result()) {
            $sql_state->close();
            return null;
        }
        if($sql_state->num_rows != 1) {
            $sql_state->close();
            return null;
        }

        $data = array();
        if(!$sql_state->bind_result($data["username"],
                                    $data["password"],
                                    $data["banned"],
                                    $data["fail_counter"],
                                    $data["cpu"],
                                    $data["computer_name"],
                                    $data["physical_hdd_serial"],
                                    $data["hwid_hash"]
        ) ) {
            $sql_state->close();
            return null;
        }
        if(!$sql_state->fetch()) {
            $sql_state->close();
            return null;
        }

        $sql_state->close();
        return $data;
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Ban a user.
    ///
    /// @param[in]  $username  The username
    ///
    /// @return     True if success, false otherwise.
    ///-------------------------------------------------------------------------------------------------
    public function ban_user($username) {
        if(!$this->m_Database) {
            return false;
        }

        $sql_state = $this->m_Database->prepare("UPDATE `userlist` SET `banned` = 1 WHERE `username` = ?");
        if(!isset($sql_state)) {
            return false;
        }
        if(!$sql_state->bind_param('s', $username)) {
            $sql_state->close();
            return false;
        }
        
        $updated = $sql_state->execute();
        $sql_state->close();
        return $updated;
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Unban a user.
    ///
    /// @param[in]  $username  The username
    ///
    /// @return     True if success, false otherwise.
    ///-------------------------------------------------------------------------------------------------
    public function unban_user($username) {
        if(!$this->m_Database) {
            return false;
        }

        $sql_state = $this->m_Database->prepare("UPDATE `userlist` SET `banned` = 0 WHERE `username` = ?");
        if(!isset($sql_state)) {
            return false;
        }
        if(!$sql_state->bind_param('s', $username)) {
            $sql_state->close();
            return false;
        }
        
        $updated = $sql_state->execute();
        $sql_state->close();

        return $updated ? $this->reset_user_failure_counter($username) : false;
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Updates the password of an user.
    ///
    /// @param[in]  $username  The username
    /// @param[in]  $password  The password
    ///
    /// @return     True if the table could be updated, false otherwise.
    ///-------------------------------------------------------------------------------------------------
    public function update_password($username, $password) {
        if(!$this->m_Database) {
            return false;
        }

        $sql_state = $this->m_Database->prepare("UPDATE `userlist` SET `password` = ? WHERE `username` = ?");
        if(!isset($sql_state)) {
            return false;
        }
        if(!$sql_state->bind_param('ss', $password, $username)) {
            $sql_state->close();
            return false;
        }
        
        $updated = $sql_state->execute();
        $sql_state->close();
        return $updated;
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Updates the hardware id of an user.
    ///
    /// @param[in]  $username             The username
    /// @param[in]  $cpu                  The cpu
    /// @param[in]  $computer_name        The computer name
    /// @param[in]  $physical_hdd_serial  The physical hdd serial
    ///
    /// @return     True if the table could be updated, false otherwise.
    ///-------------------------------------------------------------------------------------------------
    public function update_hwid($username, $cpu, $computer_name, $physical_hdd_serial) {
        if(!$this->m_Database     ||
           !isset($username)      ||
           !isset($cpu)           ||
           !isset($computer_name) ||
           !isset($physical_hdd_serial)) {
            return false;
        }

        $sql_state = $this->m_Database->prepare("UPDATE `userlist` SET `cpu` = ?, `computer_name` = ?, `physical_hdd_serial` = ?, `hwid_hash` = ? WHERE `username` = ?");
        if(!isset($sql_state)) {
            return false;
        }
  
        $hwid_hash = self::build_hwid_hash($username, $cpu, $computer_name, $physical_hdd_serial);
        if(!$sql_state->bind_param('sssss',
                                   $cpu,
                                   $computer_name,
                                   $physical_hdd_serial,
                                   $hwid_hash,
                                   $username
        )) {
            $sql_state->close();
            return false;
        }
        
        $updated = $sql_state->execute();
        $sql_state->close();
        return $updated;
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Resets the user failure counter for an user.
    ///
    /// @param[in]  $username  The username
    ///
    /// @return     True if the table could be updated, false otherwise.
    ///-------------------------------------------------------------------------------------------------
    public function reset_user_failure_counter($username) {
        if(!$this->m_Database ||
           !isset($username)) {
            return false;
        }

        $sql_state = $this->m_Database->prepare("UPDATE `userlist` SET `fail_counter` = 1 WHERE `username` = ?");
        if(!isset($sql_state)) {
            return false;
        }
        if(!$sql_state->bind_param('s', $username)) {
            $sql_state->close();
            return false;
        }
        $updated = $sql_state->execute();
        $sql_state->close();
        return $updated;
    }

    ///-------------------------------------------------------------------------------------------------
    /// @brief      Increment the failure counter for an user.
    ///
    /// @param[in]  $username  The username
    ///
    /// @return     True if the table could be updated, false otherwise.
    ///-------------------------------------------------------------------------------------------------
    public function increment_user_failure_counter($username) {
        $user_data = $this->query_user_data($username);
        if(!$user_data) {
            return false;
        }

        $num_of_failures = $user_data["fail_counter"];
        if($num_of_failures >= 3) {
            return $this->ban_user($username);
        }

        $sql_state = $this->m_Database->prepare("UPDATE `userlist` SET `fail_counter` = ? WHERE `username` = ?");
        if(!isset($sql_state)) {
            return false;
        }

        $num_of_failures++;
        if(!$sql_state->bind_param('is', $num_of_failures, $username)) {
            $sql_state->close();
            return false;
        }
        $updated = $sql_state->execute();
        $sql_state->close();
        return $updated;
    }
}
