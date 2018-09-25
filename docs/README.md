# Qub3d's Documentation!
@htmlonly
<table>
	<tr>
		<td><a href="https://www.travis-ci.org/qub3d/qub3d"><img src="https://www.travis-ci.org/qub3d/qub3d.svg?branch=develop"></a></td>
		<td style="padding-left:10px;"><img src="https://ci.appveyor.com/api/projects/status/s5ier6fjuwpv5qor?svg=true"></td>
	</tr>
</table>
@endhtmlonly

This is the documentation for the Qub3d project!

## To Build The Repositories:
Run:
```
cmake -H. -Bbuild
```
This will generate configurations into a directory called "build"

Now run:
```
cmake --build build
```
and this will actually build the project.

Things like the client, launcher and server will be built into ``COMPILE/bin``
Things like the engine and renderer will be built into ``COMPILE/lib``

