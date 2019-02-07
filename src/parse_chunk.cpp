/*
The chunk parser returns:

err=1 - all kind of errors;

end_of_chunk=1 - the chunk is read Ok. There is no any further data after.
Need to read more data from the socket. It is not an error (err=0);

short_length_data=1 - the chunk is read Ok but information
about the length of the next chunk is incomplete. This is an error (err=1);


end_of_file=1 - The chunk is parsed till the end. Nothing to read from the socket;

short_string=1 - The length of the string received is shorter than the chunk length
determined in the chunk header
*/


#include "parse_chunk.h"

parse_chunk::parse_chunk ()
{

}

parse_chunk::parse_chunk(const std::string &response1, int a)
{
   header = response1;
   next_chunk = chop_head(header, (header.find("\r\n\r\n")+4));
   err = retrieve_chunk_size();

}

parse_chunk::parse_chunk (const std::string &reply2, int *chunk_start_pointer, int *chunk_length, int &cpi)
{
    err=0;
    end_of_chunk=0;
    end_of_file=0;
    short_string=0;
    short_length_data=0;

    //Checking if the string is long enough to be parsed
    if (reply2.length() < (chunk_start_pointer[cpi] + chunk_length[cpi]))
    {
        err = 1;
        short_string = 1;
        return;
    }

    //retrieving the current chunk
    my_chunk = reply2.substr(chunk_start_pointer[cpi] + chunk_length[cpi]);

    //Checking if the chunk is long enough to be parsed
    if (my_chunk.length() > 4)
        {
            parse (my_chunk);
            string_to_recalc = my_chunk;
        }
    else
        {
            std::cout << "The length is too short:\r\n" << my_chunk.length() << std::endl;
            err = 1;
            end_of_chunk = 1;
            short_string = 1;
            return;
        }


//When the chunk is parsed without errors
if (!err)

    {

    //Determining if the chunk has been already transfered
    recalc2(reply2, chunk_start_pointer, chunk_length,
                             next_transfer_length, cpi);

          //recalculating chunk positions and next transfer length
          while (next_transfer_length < 0)
          {
              int err2 = recalc(reply2, chunk_start_pointer, chunk_length,
                             next_transfer_length, cpi);

              if (err2 == 1)
              {
                  if (!end_of_file)
                  {
                  std::cout << "Reading more1... " << std::endl;

                  return;
                  }
                  else
                  {

                  std::cout << "\r\nEnd of file... " << std::endl;

                  return;
                  }
              }
          }


        if (next_transfer_length == 0)
        {
            end_of_chunk = 1;
            std::cout << "\r\nEnd of chunk. To read: " << next_transfer_length << std::endl;
        }

        else
        {
            std::cout << "Ready to read: " << next_transfer_length << std::endl;
        }


    }
    else
    {

    short_length_data = 1;
        std::cout << "Reading more2... " << std::endl;
        return;
    }
}

void parse_chunk::parse (const std::string &my_chunk)
{
    next_chunk = my_chunk;
    offset_corrector = 0;
    end_of_chunk = 0;
    end_of_file = 0;
    std::cout << "Next part beggining " << next_chunk << std::endl;
    //Counting the number of "\r\n" in the string
    int a = count_crlf (my_chunk);
    std::cout << "The number of crlf " << a << std::endl;
    if (a < 2){err = 1;}
    else {err = 0;}
    std::cout << "err " << err << std::endl;
    if (!err)
    {
        //Removing first "\r\n" then retrieving and parsing the first string
        //(chunk length)

        //offset_corrector = 0;
        std::cout << "Initial offset corrector: " << offset_corrector << std::endl;


        next_chunk = next_chunk.substr(2 + offset_corrector);

        if (count_crlf (next_chunk) > 1)
        {
            while (next_chunk.find ("\r\n") == 0)
            {
                offset_corrector += 2;
                next_chunk = next_chunk.substr(2);
                std::cout << "Offset corrector: " << offset_corrector << std::endl;
            }
        }

        err = retrieve_chunk_size();

     }


}


int parse_chunk::count_crlf (std::string string_to_count)
{
    //set the counter to 0
    int count1 = 0;
    //define offset of the first crlf (.find returns -1 if there isn't any)
    int crlf_offset = string_to_count.find("\r\n");
    if (crlf_offset == -1)
        {
            return count1;
        }
    else
        {
         do
         {
             count1 +=1;
             string_to_count = string_to_count.substr(crlf_offset +2);
             crlf_offset = string_to_count.find("\r\n");
         }
         while (crlf_offset != -1);
         return count1;
        }
}

int parse_chunk::recalc (std::string reply2, int *chunk_start_pointer, int *chunk_length, int &next_transfer_length, int &cpi)
{

    std::cout << "Recalc says: " << chunk_start_pointer[cpi] <<", "<< chunk_length[cpi] << std::endl;
    std::cout << "String to recalc: " << string_to_recalc << std::endl;
    string_to_recalc = string_to_recalc.substr(2+chunk_bytes_to_read.length() + 2 + chunk_bytes + offset_corrector);
    std::cout << "String to recalc after correction: " << string_to_recalc << std::endl;

    parse (string_to_recalc);

    if (end_of_file){return 1;}

    if (err)
        {

        std::cout << "\r\nError: chunk length data is not complete" << std::endl;
        short_length_data = 1;

        return 1;
        }

    recalc2(reply2, chunk_start_pointer, chunk_length,
                             next_transfer_length, cpi);
    //if (end_of_file){return 1;}

    return 0;
}

void parse_chunk::recalc2 (std::string reply2, int *chunk_start_pointer, int *chunk_length, int &next_transfer_length, int &cpi)
{


    cpi +=1;
    chunk_length[cpi] = chunk_bytes;
    chunk_start_pointer[cpi] = chunk_start_pointer[cpi-1] + chunk_length[cpi-1] + 2 + offset_corrector + chunk_bytes_to_read.length() + 2;

    if (chunk_length[cpi] == 0)
    {
        err = 1;
        end_of_file = 1;
        return;
    }
    else
    {


    next_transfer_length = chunk_length[cpi] - (reply2.length() - chunk_start_pointer[cpi]);
    std::cout << "chunk_start_pointer " << cpi << ": " << chunk_start_pointer[cpi] << std::endl;
    std::cout << "Next part: " << reply2.substr (chunk_start_pointer[cpi]) << std::endl;
    std::cout << "Previous part: " << reply2.substr (chunk_start_pointer[cpi-1]) << std::endl;

    }
    /*std::cout << "Recalc says: " << chunk_start_pointer[cpi] <<", "<< chunk_length[cpi] << std::endl;
    cpi +=1;
    std::cout << "String to recalc: " << string_to_recalc << std::endl;
    string_to_recalc = string_to_recalc.substr(2+chunk_bytes_to_read.length() + 2 + chunk_bytes);
    std::cout << "String to recalc after correction: " << string_to_recalc << std::endl;
    parse (string_to_recalc);


    chunk_start_pointer[cpi] = 100;
    chunk_length[cpi] = 200;

*/
    //return 0;
}

bool parse_chunk::retrieve_chunk_size ()

{
   if (next_chunk.find("\r\n") != -1)
   {
   chunk_bytes_to_read = next_chunk.substr(0, next_chunk.find("\r\n"));
   std::cout << "chunk_bytes_to_read HEX " << next_chunk.substr(0, next_chunk.find("\r\n"));

   chunk_bytes = hex_convert(chunk_bytes_to_read);
   if (chunk_bytes == -1){return 1;}
   else if (chunk_bytes == 0)
   {
       end_of_file = 1;
       return 1;
   }
   else
    {return 0;}
   }
   else {return 1;}
}
//Converts std::string (hex) to unsigned int
int parse_chunk::hex_convert (const std::string &my_chunk)
{
    try
    {
       unsigned int x = std::stoul(my_chunk, nullptr, 16);
       return x;
    }
    catch (...)
    {
        return -1;
    }
}




// finds and returns text right after the header "\r\n\r\n"
std::string parse_chunk::chop_head (std::string my_chunk, int chunk_length)
{
    std::string string1 = my_chunk.substr(chunk_length, (my_chunk.length() - chunk_length));
    return string1;
}


//finds and returns string which begins with str_to_find

std::string parse_chunk::find_str (const std::string &str_to_find)
{
    a = header.find (str_to_find);

    if (a !=-1){
            std::string string2 = header.substr(a, header.length()-a);
            a = string2.find ("\r\n");
            std::string string3 = string2.substr(0, a);
            err = 0;
            return string3;
    }
    err = 1;
    return "Cannon find element";
}
//returns the value after the colon
std::string parse_chunk::find_val (const std::string &str_to_find)
{
    a = str_to_find.find (":")+2;
    std::string string2 = str_to_find.substr(a, str_to_find.length()-a);

    return string2;
}

