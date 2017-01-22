<?php

///-------------------------------------------------------------------------------------------------
/// @brief      Error codes
///-------------------------------------------------------------------------------------------------
class eLicenseCode
{
    const Valid                         =  0;
    const Exception                     =  1;
    const FailedToSendRequest           =  2;
    const FailedToParseResponse         =  3;
    const InvalidHwid                   =  4;
    const InvalidPassword               =  5;
    const InvalidUser                   =  6;
    const Banned                        =  7;
    const FailedToBuildHwidHash         =  8;
    const Server_FailedToEncryptData    =  9;
    const MySQL_FailedToUpdatePassword  = 10;
    const MySQL_FailedToIncremetCounter = 11;
    const MySQL_FailedToResetCounter    = 12;
    const MySQL_FailedToUpdateHwid      = 13;
}
