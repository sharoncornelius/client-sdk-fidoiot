# FIDO Device Onboard (FDO) Compilation Setup
<a name="safestring"></a>
## 1. Intel safestringlib
FDO Client SDK uses safestringlib for string and memory operations to prevent serious security vulnerabilities (For example, buffer overflows).

1. For Linux*/ Arm Mbed* OS/ Arm Mbed Linux OS builds, download safestring from <a href="https://github.com/intel/safestringlib">intel-safestringlib</a>.

<a name="tinycbor"></a>
## 2. Intel TinyCBOR
FDO Client SDK uses TinyCBOR library for Concise Binary Object Representation (CBOR) encoding and decoding.

2. For Linux* OS builds, download TinyCBOR from <a href="https://github.com/intel/tinycbor">tinycbor</a>.

<a name="manuf_addr"></a>
## 3. Setting the Manufacturer Network Address
To set the manufacturer network address(domain name or IP) that FDO Client SDK Linux device uses during Device Initialization (DI) protocol:

```shell
# To set the manufacturer DNS
$ cd <path-to-client-sdk-fidoiot>
$ echo -n <manufacturer domain-name> > data/manufacturer_dn.bin
```
or

```shell
# To set the manufacturer IP
$ cd <path-to-client-sdk-fidoiot>
$ echo -n <manufacturer server-ip> > data/manufacturer_ip.bin
```

The default manufacturer port is 8039. If required, it can be configured by following instructions:

```shell
# For setting manufacturer port
$ cd <path-to-client-sdk-fidoiot>
$ echo -n <manufacturer server-port> > data/manufacturer_port.bin
```

> ***Note:*** By default, `manufacturer_dn.bin` is configured with "localhost". If both IP and domain name are set, the IP takes precedence over domain name.

<a name="ecdsa_priv"></a>
## 4. Elliptic Curve Digital Signature Algorithm (ECDSA) Private Key File Generation
The following are steps to generate the private key file for ECDSA-based devices, only EC Curve `P-256` and `P-384` are supported.

*  Generate EC private key (optional, if not already generated):
   ```shell
   $ openssl ecparam -name prime256v1 -genkey -noout -out key.pem #For P-256
   ```
   or
   ```shell
   $ openssl ecparam -name secp384r1 -genkey -noout -out key.pem #For P-384
   ```

*   **Option1 (default):** To generate the binary private key data file

1. To parse the Privacy-Enhanced Mail (PEM)-formatted EC private key to generate the private key in binary format:
   ```shell
   $ openssl asn1parse < key.pem
   ```
   > ***Note***: An example of output from the preceding command: <br>
     ...... <br>
     ...... <br>
     .. [HEX DUMP]:A253014C61B6AEB5FA867B5417CD4A87D45BD6A505E81060D064529D0540CD36<br>
     ...... <br>
     ...... <br>

2. Use the `[HEX DUMP]` information from the above to generate the respective EC key (.dat) file (`ecdsa256privkey.dat` for EC curve P-256 and `ecdsa384privkey.dat` for EC curve P-384):
	For example,
   ```shell
   $ echo 'A253014C61B6AEB5FA867B5417CD4A87D45BD6A505E81060D064529D0540CD36' | xxd -r -p > ecdsaXXXprivkey.dat
   ```

   The respective ecdsaXXXprivkey.dat file will be used by the FDO target binary (Linux* or Arm* Mbed* OS) while ECDSA sign operation.
   

* **Option2:** To use the private key in PEM format, rename key.pem to ecdsaXXXprivkey.pem (`ecdsa256privkey.pem` for EC curve P-256 and `ecdsa384privkey.pem` for EC curve P-384). Use the compilation flag `DA_FILE=pem` during binary creation.

## 5.  Setting the maximum ServiceInfo size

The maximum permissible ServiceInfo size (both Device and Owner) that FDO Client SDK can process should be set in the file `max_serviceinfo_sz.bin`. The value must lie between 1300 and 8192 (both inclusive). If the set value is less than 1300, the value would default to 1300. Similarly, if the value is greater than 8192, the value would default to 8192.

This value is sent as TO2.DeviceServiceInfoReady.maxOwnerServiceInfoSz and is compared with the TO2.OwnerServiceInfoReady.maxDeviceServiceInfoSz.

```shell
# To set the manufacturer DNS
$ cd <path-to-client-sdk-fidoiot>
$ echo -n <integer size> > data/max_serviceinfo_sz.bin
```

## 6.  FDO Credentials REUSE Protocol

The FDO credentials REUSE feature allows FDO devices to reuse their ownership credentials across multiple device onboardings. This feature only gets enabled if the owner sends down the same rendezvous info, device GUID information, and public key at the end of the Transfer of Ownership, Step 2 (TO2) protocol.

Specifically, if TO2.SetupDevice.TO2SetupDevicePayload.RendezvousInfo, TO2.SetupDevice.TO2SetupDevicePayload.Guid, and TO2.SetupDevice.TO2SetupDevicePayload.Owner2Key match the corresponding values held by the device, the device will not generate a Hash-based Message Authentication Code (HMAC), which then allows the original ownership voucher (OV) to be used for another (and subsequent) onboarding(s) by reusing the same device credentials multiple times.

However, device client binary must be generated using -DREUSE=true flag:

```shell
$ cmake -DREUSE=true
```
Activating the device credentials will in turn, activate the FDO device and configure the FDO device to run multiple onboarding(s). This can be useful in several test and development environments, where multiple onboardings are common.

If REUSE flag is set to false, and the above mentioned conditions for credential REUSE are met, an error message will be thrown. This can be useful for a scenario where the device credentials should never be reused, once saved.

<a name="http_proxy"></a>
## 7. HTTP-proxy configuration (optional)
If the device is located behind a proxy server, the proxy server details must be provided to the device. For the same purpose, there are three files (each for the manufacturer, rendezvous, and owner servers) in which the proxy server details should be specified in the required format, before connecting to the respective server. These files can be created or removed as required.

Each proxy file is located in the `data/` directory and named as follows:

* `mfg_proxy.dat` - holds the proxy server network address between the device and manufacturer.
* `rv_proxy.dat` - holding the proxy server network address between the device and the rendezvous server.
* `owner_proxy.dat` - holds the proxy server network address between the device and owner.

The following is the format for proxy server network address:

    <Proxy Server IP>:<proxy Server Port>  e.g. 255.255.255.255:65535

> ***Note:*** The files `rv_proxy.dat`,`mfg_proxy.dat`, and `owner_proxy.dat` must not contain any other information beyond the information mentioned above.

The proxy server network address is optional if the device connects to an access point that connects the device through the proxy server.

***Note :***  FDO clients that run on the Linux* OS also support network proxy discovery using the environment variable or the Web Proxy Auto-Discovery (WPAD) protocol based on the libproxy library. To use wpad protocol, use export http_proxy=’wpad:’.
