# VisualCrypt

With the program "visualCrypt" images in .bmp format can be encrypted.

## Generate Program

In the ./source directory is a makefile, which creates the program "visualCrypt" by calling
> make  

Since the program uses Linux-specific libraries, it is recommended to use it on a Linux  
operating system (or subsystem).

## Call Program

The executable program can then be found in the./source directory.  
Calling it with transfer parameters is optional.

### Examples Of Usage

> ./visualCrypt

For execution from the source directory.

> ./source/visualCrypt

For execution from the VisualCrypt directory.

#### The following transfer parameters are defined for the program

> ./source/visualCrypt -h

Provides information about usage.

>./source/visualCrypt -s &lt;path to image file&gt;

With -s it is possible to select the image to be encrypted.  
After the option, the path to a valid BMP file must be passed.  
Without the parameter, the image &lt;path to visualCrypt&gt;/image/cameraman.bmp is used.

>./source/visualCrypt -d &lt;path to target directory&gt;

With -d you can select the target directory in which the generated shares can be stored  
or searched (for decryption). The specified folder must already exist for this purpose.  
The batch results of the shares, after decryption, are also stored there.  
Without the parameter, the directory &lt;path to visualCrypt&gt;/image is used.

### Program Menu

Option points 1 to 5 provide different algorithms for encryption of a BMP file.  
After selecting an algorithm, the number of shares to be generated is always requested.

With option point 6 already created shares can be decrypted again.  
Since the names of the shares are "share01.bmp", "share02.bmp", etc.,  
the numbers of the first and last shares must then be specified, which should be part  
of the decryption.

Option point 7 starts a time measurement across all algorithms.  
Attention: The default setting may take several hours!  
By default, the result of the time measurement is named "timeMeasurement.log",  
and is stored in the main directory of the program (visualCrypt folder).

### Other Options

In "settings.h" options are outsourced that only need to be adjusted to a limited extent.  
This includes, for example, the "Threshold", which influences the contrast of the batch result.  
It is also possible in here to edit the number of measurement repeats in the time measurement.

## Delete Program

If the command:
> make clean

 is used in the source directory, all compilations are deleted.