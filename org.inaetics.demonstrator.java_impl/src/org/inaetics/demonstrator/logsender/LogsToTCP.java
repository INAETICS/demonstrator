/**
 * Licensed under Apache License v2. See LICENSE for more information.
 */
package org.inaetics.demonstrator.logsender;

import java.io.PrintWriter;
import java.io.Writer;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.Date;
import java.util.Dictionary;

import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.log.LogEntry;
import org.osgi.service.log.LogListener;
import org.osgi.service.log.LogReaderService;

/**
 * A {@link LogListener} which registers itself to the {@link LogReaderService},
 * in order to send {link LogEntry}s to logstash
 */
public class LogsToTCP implements LogListener, ManagedService {

	public static final String PID = "org.inaetics.demonstrator.logsender";
	private static final String PROPERTY_LOGSTASH_HOST = "logstash.host";
	private static final String PROPERTY_LOGSTASH_PORT = "logstash.port";
	
	// injected by Felix DM
	private volatile LogReaderService m_logReader;
	
	private String m_logstashHost;
	private int m_logstashPort = -1;

	private DateTimeFormatter m_formatter;
	private String m_hostname;

	private Socket m_socket;
	private Writer m_writer;

	public LogsToTCP() {
		try {
			// get own hostname
			m_hostname = InetAddress.getLocalHost().getHostName();
		} catch (UnknownHostException e) {
			m_hostname = "unknownHost";
		}
		m_formatter = DateTimeFormatter.ISO_LOCAL_DATE_TIME;
	}

	// called by Felix DM
	protected final void start() throws Exception {
		m_logReader.addLogListener(this);
	}
	
	// called by Felix DM
	protected final void stop() throws Exception {
		m_logReader.removeLogListener(this);
		cleanup();
	}

	@Override
	public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
		if (properties == null) {
			setConfiguration(null, -1);
			cleanup();
			return;
		}
		if (properties.get(PROPERTY_LOGSTASH_HOST) != null
				&& properties.get(PROPERTY_LOGSTASH_PORT) != null) {
			try {
				String host = properties.get(PROPERTY_LOGSTASH_HOST).toString();
				int port = Integer.parseInt(properties.get(PROPERTY_LOGSTASH_PORT).toString());
				setConfiguration(host, port);
			}
			catch (Exception e) {
				throw new ConfigurationException(null, e.getMessage());
			}
		}
	}

	/**
	 * @param logstashHost the host on which logstash is accepting logs via TCP
	 * @param logstashPort the host on which logstash is accepting logs via TCP
	 */
	private void setConfiguration(String logstashHost, int logstashPort) {
		m_logstashHost = logstashHost;
		m_logstashPort = logstashPort;
	}

	@Override
	public void logged(LogEntry entry) {
		try {
			
			// try to get a writer
			Writer writer = getWriter();
			if (writer == null) {
				// silently fail, ELK service might just not be running
				return;
			}
			
			// prepare a exception string if the log contains an exception
			String exception = entry.getException() == null ? "" : " " + entry.getException().getMessage();
			
			// format time of log
			Instant dateTimeInstant = new Date(entry.getTime()).toInstant();
			LocalDateTime dateTime = LocalDateTime.ofInstant(dateTimeInstant, ZoneId.of("GMT"));
			
			// concatenate log string
			String log = m_formatter.format(dateTime)
					+ " " + m_hostname
					+ " " + entry.getBundle().getSymbolicName()
					+ " " + getLogLevel(entry.getLevel())
					+ " " + entry.getMessage()
					+ exception;
			
			// write log string to logstash
			writer.write(log + "\n");
			writer.flush();
		}
		catch (Exception e) {
			// silently fail, ELK service might just not be running
			cleanup();
		}
	}
	
	private Writer getWriter() {
		try {
			if ((m_socket == null || m_socket.isClosed() || m_writer == null)
					&& m_logstashHost != null && m_logstashPort > 0) {
				m_socket = new Socket(m_logstashHost, m_logstashPort);
				m_writer = new PrintWriter(m_socket.getOutputStream(), true);
			}
		}
		catch (Exception e) {
			// silently fail, ELK service might just not be running
			cleanup();
		}
		return m_writer;
	}
	
	private void cleanup() {
		try {
			if (m_writer != null) {
				m_writer.close();
			}
			if (m_socket != null) {
				m_socket.close();
			}
		} catch (Exception e) {
			// nothing to do
		}
		finally {
			m_writer = null;
			m_socket = null;
		}
	}
	
	private String getLogLevel(int level) {
		switch (level) {
			case 1: return "ERROR"; 
			case 2: return "WARN"; 
			case 3: return "INFO"; 
			case 4: return "DEBUG";
			default: return "UNKNOWN";
		}
	}

}
