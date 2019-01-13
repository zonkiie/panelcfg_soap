# panelcfg_soap
Soap Interface for System Config Panels

This Program is written in C++.
It uses gSOAP and Boost Libraries.
Maybe a plain ansi C version will follow if there is enough time in the future.

## License

This Program is licensed under the GPLv2 license, the same license as gSOAP.

## Prerequisites

A g++ version which can compile c++x11 code
liboost-all
gsoap

The apache tools a2ensite, a2dissite, apache2ctl must be in the /usr/sbin directory, as well as the useradd/usermod/userdel tools.

As we need to authenticate, the Program needs to run as process, not as cgi.
This could create some problems with older gSOAP Versions on older Linux systems.
On the developer's machine for example the compiled program would not start because the protocol IPV6ONLY was unknown.

If a user is in the group "admin", he gets all permissions to the operations.
