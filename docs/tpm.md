# Linux* TPM* Implementation

`Ubuntu* OS version 20.04` on x86 was used as a development and execution OS. Follow these steps to compile and execute FIDO Device Onboard (FDO).

The FDO build and execution depend on OpenSSL* toolkit version 1.1.1g. Users must install or upgrade the toolkit before compilation if the toolkit is not available by default in the environment.

## 1. Packages Requirements when Setting up TPM2.0 (on Ubuntu* OS version 20.04):

OpenSSL* toolkit version 1.1.1g. Follow the steps given in Section 10 to update the openssl version to 1.1.1g.

## 2. TPM* Library Installation (for Ubuntu OS version 20.04):

TPM enabled FDO Client SDK uses TPM-TSS 2.3.1, TPM2-ABRMD 2.2.0 and TPM2-TOOLS 4.0.1 libraries for key and cryptography related operations. The TPM-TSS library is required for compiling the code while all 3 libraries are required for running the code. Create an empty directory, download and execute FDO TPM [TPM-Library-Installation-Script](../utils/install_tpm_libs.sh) which can be used for both installation and uninstallation of TPM libraries. Alternatively, perform steps listed in section 2.1 to setup TPM library without using the TPM [TPM-Library-Installation-Script](../utils/install_tpm_libs.sh).

To compile and execute TPM enabled FDO Client SDK use one of the appropriate commands:

* Script usage command

```shell
$ ./install_tpm_libs.sh -h
```

* TPM-TSS library setup to enable TPM enabled FDO Client SDK code compilation

```shell
# Command to install tpm-tss library
$ ./install_tpm_libs.sh -t

# Command to uninstall tpm-tss library
$ ./install_tpm_libs.sh -d
```

* TPM setup to enable TPM enabled FDO Client SDK code compilation and execution

```shell
# Command to install TPM libraries
$ ./install_tpm_libs.sh -i

# Command to uninstall TPM libraries
$ ./install_tpm_libs.sh -u
```
> ***Note:*** Installation of these components may require elevated permissions. Please use 'sudo' to execute the script.

### 2.1 Building and Installing Libraries for Trusted Platform Module (TPM)

Following steps should be performed if FDO TPM [TPM-Library-Installation-Script](../utils/install_tpm_libs.sh) script is not used to setup FDO TPM libraries. Install only tpm2-tss library to enable TPM enabled FDO Client SDK code compilation. To enable compilation and execution of TPM enabled FDO Client SDK code, install all libraries namely: tpm2-tss, tpm2-abrmd, tpm2-tools, and tpm2-tss-engine.

#### 2.1.1 tpm2-tss-2.3.1

This is the main library that creates commands per Trusted Computing Group (TCG) specification to use the TPM. It uses release version 2.3.1 of the library.

##### Source Code

The library can be downloaded from [tpm2-tss-2.3.1-download](https://github.com/tpm2-software/tpm2-tss/releases/download/2.3.1/tpm2-tss-2.3.1.tar.gz)

##### Build and Install Process

The build and installation process can be found at [tpm2-tss-2.3.1-install](https://github.com/tpm2-software/tpm2-tss/blob/2.3.x/INSTALL.md)

#### 2.1.2 tpm2-abrmd-2.2.0

This is an optional but recommended library (daemon) to use TPM in the device. This daemon will act as a resource manager for the TPM, for all I/O calls that happen with the device. It uses release version 2.2.0 of the library.

##### Source Code

The library can be downloaded from [tpm2-abrmd-2.2.0-download](https://github.com/tpm2-software/tpm2-abrmd/releases/download/2.2.0/tpm2-abrmd-2.2.0.tar.gz)

Alternatively, the in-kernel RM /dev/tpmrm0 can be used. Please see Section on Compiling FDO.

##### Build and Install Process

The build and installation process found at [tpm2-abrmd-2.2.0-install](https://github.com/tpm2-software/tpm2-abrmd/blob/master/INSTALL.md)

#### 2.1.3 tpm2-tools-4.0.1

This library provides the necessary tools to interact and perform operations using the TPM, to the users. It uses release version 4.0.1 of the library.

##### Source Code

The library can be downloaded from [tpm2-tools-4.0.1-download](https://github.com/tpm2-software/tpm2-tools/releases/download/4.0.1/tpm2-tools-4.0.1.tar.gz)

##### Build and Install Process

The build and installation process can be found at [tpm2-tools-4.0.1-install](https://github.com/tpm2-software/tpm2-tools/blob/4.0.X/INSTALL.md)

#### 2.1.4 tpm2-tss-engine-1.1.0

This library provides the OpenSSL engine, which performs the OpenSSL cryptography operation using the keys inside the TPM. It uses release version 1.1.0 of the library.

##### Source Code

The library can be downloaded from [tpm2-tss-engine-download](https://github.com/tpm2-software/tpm2-tss-engine/archive/v1.1.0.zip)

##### Build and Install Process

The build and installation process can be found at [tpm2-tss-engine-install](https://github.com/tpm2-software/tpm2-tss-engine/blob/v1.1.0/INSTALL.md)

## 3. Compiling Intel safestringlib

FDO Client SDK uses safestringlib for string and memory operations to prevent serious security vulnerabilities (For example, buffer overflows). Download safestringlib from <a href="https://github.com/intel/safestringlib">intel-safestringlib</a>, checkout to the tag `v1.0.0` and follow these instructions to build:
From the root of the safestringlib, do the following:
 ```shell
 $ mkdir obj
 $ make
 ```
After this step, `libsafestring.a` library will be created.

## 4. Compiling Intel TinyCBOR
FDO Client SDK uses TinyCBOR library for Concise Binary Object Representation (CBOR) encoding and decoding. Download TinyCBOR from <a href="https://github.com/intel/tinycbor">TinyCBOR</a>, checkout to the tag `v0.5.3` and follow these instructions to build:
From the root of the TinyCBOR (named `tinycbor`), do the following:
 ```shell
 $ make
 ```

## 5. Environment Variables

Add these environment variables to ~/.bashrc or similar (replace with actual paths).
Provide safestringlib and tinycbor path:
```shell
$ export SAFESTRING_ROOT=path/to/safestringlib
$ export TINYCBOR_ROOT=path/to/tinycbor
```

## 6. Compiling ServiceInfo Modules (optional)

Provide the ServiceInfo device module path to use the FDO ServiceInfo functionality:
```shell
$ export SERVICE_INFO_DEVICE_MODULE_ROOT=path/to/service_info_module_dir
```
ServiceInfo device module `*.a` must be present in the `SERVICE_INFO_DEVICE_MODULE_ROOT`, that is, required ServiceInfo device modules must be built prior to this step, otherwise the FDO Client SDK build will fail.

## 7. Compiling FDO Client SDK

The FDO Client SDK build system is based on <a href="https://www.gnu.org/software/make/">GNU make</a>.  assumes that all the requirements are set up according to [ FDO Compilation Setup ](setup.md). The application is built using the `make [options]` in the root of the repository for all supported platforms. The debug and release build modes are supported in building the FDO Client SDK.

Refer the TPM Library Setup steps given in Section 2 to compile TPM enabled FDO Client SDK. 

For an advanced build configuration, refer to [ Advanced Build Configuration ](build_conf.md). 

Example command to build TPM enabled FDO Client SDK with the Resource Manager as TPM2-ABRMD (tabrmd)

```shell
make pristine
cmake -DPK_ENC=ecdsa -DDA=tpm20_ecdsa256 .
make -j$(nproc)
```

To use the in-kernel Resource Manager '/dev/tpmrm0', use the following command
```shell
make pristine
cmake -DPK_ENC=ecdsa -DDA=tpm20_ecdsa256 -DTPM2_TCTI_TYPE=tpmrm0 .
make -j$(nproc)
```

Several other options to choose when building the device are, but not limited to, the following: device-attestation (DA) methods, Advanced Encryption Standard (AES) encryption modes (AES_MODE), key-exchange methods (KEX), Public-key encoding (PK_ENC) type, and SSL support (TLS).
Refer to the section [FDO Build configurations](build_conf.md)

<a name="run_linux_fdo"></a>

## 8. Running the Application <!-- Ensuring generic updates are captured where applicable -->
The TPM enabled FDO Client SDK Linux device is compatible with  FDO PRI components - Manufacturer, Reseller, Rendezvous, and Owner.

To test the FDO Client SDK Linux device, setup the [FDO PRI Manufacturer](https://github.com/secure-device-onboard/pri-fidoiot/blob/master/component-samples/demo/manufacturer/README.md),
[FDO PRI Rendezvous](https://github.com/secure-device-onboard/pri-fidoiot/blob/master/component-samples/demo/rv/README.md) and
[FDO PRI Owner](https://github.com/secure-device-onboard/pri-fidoiot/blob/master/component-samples/demo/owner/README.md).

Refer the TPM Library Setup steps given in Section 2 to compile and execute TPM enabled FDO Client SDK

After a successful compilation, the  FDO Client SDK Linux device executable can be found at `<path-to-client-sdk-fidoiot>/build/linux-client`.

- Before executing `linux-client`, prepare for Device Initialization (DI) by starting the FDO PRI Manufacturer
  Refer to [ Device Initialization Setup ](DI_setup.md).
  Then, execute the TPM make ready script. Refer to [TPM Make Ready](../utils/tpm_make_ready_ecdsa.sh).
  Alternatively, perform the steps listed in section 8.1 to initialise the device without using
  [TPM Make Ready](../utils/tpm_make_ready_ecdsa.sh) script.

  Script execution command:

  ```shell
  $ ./tpm_make_ready_ecdsa.sh -p <FDO Client SDK data folder location>
  ```

- Once the TPM make ready script is executed successfully, the device is now initialized
  with the credentials and is ready for ownership transfer. To run the device against the
  FDO PRI Manufacturer for the DI protocol, do the following:
  ```shell
  $ ./build/linux-client
  ```

- To enable the device for Transfer Ownership protocol (TO1 and TO2), configure the FDO PRI Rendezvous and Owner.
  Refer to [ Ownership Transfer Setup ](ownership_transfer.md).
  After these are set up, execute `linux-client` again.
  
  ```shell
  $ ./build/linux-client
  ```

> ***Note:*** If the `linux-client` was built with flag TPM2_TCTI_TYPE=tpmrm0, running the it along with tpm_make_ready_ecdsa.sh, may require elevated privileges. Please use 'sudo' to execute.

### 8.1 Prepare FDO Client SDK Data Folder

#### Persistent Storage Index in TPM

Find a persistent storage index that is unused in the TPM and note it down. It usually starts from 0x81000000. To see the indexes that are already being used, use the following command. FDO uses the 0x81000001 index for the following command examples.

 ```shell
  $ tpm2_getcap handles-persistent
  ```


#### Primary Key Generation from Endorsement Hierarchy

 ```shell
  $ tpm2_createprimary -C e -g sha256 -G ecc256:aes128cfb -c data/tpm_primary_key.ctx -V
  ```

#### Load the Primary Key into TPM Persistent Memory

 ```shell
  $ tpm2_evictcontrol -C o 0x81000001 -c data/tpm_primary_key.ctx -V
  ```

#### Device ECDSA Key-Pair Generation

 ```shell
  $ tpm2tss-genkey -a ecdsa -c nist_p256 data/tpm_ecdsa_priv_pub_blob.key -v -P 0x81000001
  ```

#### Generate Device MString

 ```shell
  $ export OPENSSL_ENGINES=/usr/local/lib/engines-1.1/; openssl req -new -engine tpm2tss -keyform engine -out data/device_mstring -key data/tpm_ecdsa_priv_pub_blob.key -subj "/CN=www.fdoDevice1.intel.com" -verbose; truncate -s -1 data/device_mstring; echo -n "13" > /tmp/m_string.txt; truncate -s +1 /tmp/m_string.txt; echo -n "intel-1234" >> /tmp/m_string.txt; truncate -s +1 /tmp/m_string.txt; echo -n "model-123456" >> /tmp/m_string.txt; truncate -s +1 /tmp/m_string.txt; cat data/device_mstring >> /tmp/m_string.txt; base64 -w 0 /tmp/m_string.txt > data/device_mstring; rm -f /tmp/m_string.txt
  ```

## 9. Troubleshooting Details

- TPM Authorization Failure while Running tpm2-tools Command.<br />
  Clear TPM from the BIOS. To run the TPM enabled FDO Client SDK implementation, the TPM on the device should not be owned.
  To reset the TPM, go to your device BIOS and clear the TPM. To find the location of the option in the BIOS of your 
  device, refer to your device manual.

- Clear the Used Persistent Index in TPM.<br />
  Use the tpm2_evictcontrol command to delete the content or clear TPM from the BIOS. To run the TPM-based FDO 
  implementation, the TPM on the device should not be owned. To reset the TPM, go to your device BIOS and clear the TPM.
  To find the location of the option in the BIOS of your device, refer to your device manual.

  Assuming that the index is 0x81000001, run the following command to delete the keys.

  ```shell
  $ tpm2_evictcontrol -C o -c 0x81000001 -V
  ```

- OpenSSL* Toolkit Library Linking Related Error While Building FDO Client SDK.<br />
  There is a dependency on the OpenSSL* toolkit version 1.1.1g for building and running the FDO Client SDK.
  Check the version of the OpenSSL toolkit installed in your machine with the command

  ```shell
  $ openssl version
  ```
  If the OpenSSL toolkit version in your machine is earlier than version 1.1.1g, follow the steps given in Section10 to update the OpenSSL version to 1.1.1g.

## Steps to Upgrade the OpenSSL* Toolkit to Version 1.1.1g

```shell
# 1. If libssl-dev is installed, remove it:

  $ sudo apt-get remove --auto-remove libssl-dev
  $ sudo apt-get remove --auto-remove libssl-dev:i386

# 2. Pull the tarball: 

  $ wget https://www.openssl.org/source/openssl-1.1.1g.tar.gz

# 3. Unpack the tarball with 

  $ tar -zxf openssl-1.1.1g.tar.gz && cd openssl-1.1.1g

# 4. Issue the command 

  $ ./config

# 5. Issue the command 

  $ make 
  (You may need to run `sudo apt install make gcc` before running this command successfully).

# 6. Check for possible errors.

  $ make test

# 7. Backup the current OpenSSL binary

  $ sudo mv /usr/bin/openssl ~/tmp

# 8. Issue the command

  $ sudo make install

# 9. Create a symbolic link from the newly installed binary to the default location:

  $ sudo ln -s /usr/local/bin/openssl /usr/bin/openssl

# 10.  Run the command to update symlinks and rebuild the library cache.

  $ sudo ldconfig

# 11. Assuming no errors in executing steps 4 through 10, you should have successfully installed the new version of the OpenSSL toolkit.
# Issue the following command from the terminal:

  $ openssl version

  Your output should be as follows:

  OpenSSL 1.1.1g  31 Mar 2020
```
